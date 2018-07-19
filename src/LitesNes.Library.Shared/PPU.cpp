#include "pch.h"
#include "PPU.h"
#include <ctype.h>
#include <cmath>
#include <vector>
#include "Ram.h"
#include "imgui.h"
#include "NesDebugger.h"

Ram PPU::VRam = Ram("VRam");
OAM PPU::sOAM;
Register PPU::PPUCTRL = Register("PPUCTRL");
Register PPU::PPUMASK = Register("PPUMASK");
Register PPU::PPUSTATUS = Register("PPUSTATUS");
AddressRegister PPU::OAMADDR = AddressRegister("OAMADDR");
OAMRegister PPU::OAMDATA = OAMRegister("OAMDATA", PPU::OAMADDR, PPU::sOAM);
AddressRegister PPU::PPUSCROLL = AddressRegister("PPUSCROLL");
AddressRegister PPU::PPUADDR = AddressRegister("PPUADDR");
WritingRegister PPU::PPUDATA = WritingRegister("PPUDATA", PPU::PPUADDR, PPU::VRam);
Register PPU::OAMDMA = Register("OAMDMA"); //TODO: set this sucker up to do full AOM setups
bool PPU::mRenderPatternTables = true;
bool PPU::mTableSide = true;

PPU::PPU(uint32_t chrRomSize, char* chrRomPtr)
	:mPatternTables(chrRomSize, chrRomPtr)
{
	OAM::OAMEntry& oam = sOAM.GetEntry(1);
	oam.yPos = 10;
	oam.xPos = 10;
	oam.tileIndex = 12;
	oam.attributes = 12;
}

PPU::~PPU()
{
}

void PPU::Render(uint32_t* texArray)
{
	texArray;
	for (uint32_t i = 0; i < 240; ++i)
	{
		RenderLine(i, texArray);
	}
}

void PPU::RenderLine(uint32_t lineNum, uint32_t* texArray)
{
	assert(lineNum <= 240);
	uint32_t* lineStart = texArray + (PPU::ScreenWidth * lineNum);

	std::vector<OAM::OAMEntry> oamEntries = sOAM.GetCollidingSprites(lineNum);
	if (oamEntries.size() > 8) {
		//TODO: set sprite overflow flag
	}
	DrawBackground(lineNum, lineStart);
	for( OAM::OAMEntry oamEntry : oamEntries)
	{
		DrawSpriteParts(lineNum, lineStart, oamEntry);
	}
}

void PPU::DrawBackground(uint32_t lineNum, uint32_t* lineToDrawTo)
{
	uint16_t nametableLine = uint16_t(lineNum / 8);
	uint16_t nametableStartingIndex = (256 / 8) * nametableLine;
	uint16_t xOffset = 0;
	for (uint8_t i = 0; i < 8; ++i)
	{
		PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(VRam.GetMemoryByLocation(nametableStartingIndex + i), mTableSide ? PatternTables::LEFT : PatternTables::RIGHT);
		for (uint8_t x = 0; x < 8; ++x)
		{
			uint8_t clr = curTable.GetValueByIndex(x, lineNum % 8);
			uint32_t color = 0;
			switch (clr)
			{
			case 0:
				color = 0xFFFFFFFF;
				break;
			case 1:
				color = 0x00FF00FF;
				break;
			case 2:
				color = 0xBBBBBBFF;
				break;
			case 3:
				color = 0x00FFFFFF;
				break;
			default:
				break;
			}
			lineToDrawTo[xOffset + x] = color;
		}
		xOffset += 8;
	}
}

void PPU::DrawSpriteParts(uint32_t lineNum, uint32_t* lineToDrawTo, OAM::OAMEntry& oamEntry)
{
	uint8_t row = uint8_t(std::abs(int(lineNum) - int(oamEntry.yPos)));
	PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(oamEntry.tileIndex, mTableSide ? PatternTables::RIGHT : PatternTables::LEFT);
	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t clr = curTable.GetValueByIndex(i, row);
		uint32_t color = 0;
		switch (clr)
		{
		case 0:
			continue;
			break;
		case 1:
			color = 0x00FF00FF;
			break;
		case 2:
			color = 0xBBBBBBFF;
			break;
		case 3:
			color = 0x00FFFFFF;
			break;
		default:
			break;
		}
		lineToDrawTo[oamEntry.xPos + i] = color;
	}
}

void PPU::DrawFromBuffer(uint32_t* texArray)
{
	if (mRenderPatternTables) {
		RenderPatternTables(texArray);
	} else {
		RenderNametables(texArray);
	}
}

void PPU::RenderPatternTables(uint32_t* texArray)
{
	uint8_t drawX = 0;
	uint8_t drawY = 0;
	for (uint32_t j = 0; j < 16; ++j)
	{
		for (uint32_t i = 0; i < 32; ++i)
		{
			PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(i + (j * 32));
			DrawPatternTable(texArray, curTable, drawX, drawY);
			drawX += 8;
		}
		drawY += 8;
	}
}

void PPU::RenderNametables(uint32_t* texArray)
{
	uint32_t drawX = 0;
	uint32_t drawY = 0;
	uint16_t nameTableIndex = 0;

	for (uint32_t i = 0; i < 30; ++i)
	{
		for (uint32_t j = 0; j < 32; ++j)
		{
			PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(VRam.GetMemoryByLocation(nameTableIndex), mTableSide ? PatternTables::LEFT : PatternTables::RIGHT);
			++nameTableIndex;
			DrawPatternTable(texArray, curTable, drawX % 256, drawY);
			drawX += 8;
		}
		drawY += 8;
	}
}

void PPU::DrawPatternTable(uint32_t* texArray, PatternTables::PatternTable& tableToDraw, uint32_t xOffset, uint32_t yOffset)
{
	for (uint8_t x = 0; x < 8; ++x)
	{
		for (uint8_t y = 0; y < 8; ++y)
		{
			uint32_t texArrayIndex = (xOffset + x) + (256 * (yOffset + y));
			uint8_t value = tableToDraw.GetValueByIndex(x, y);
			switch (value)
			{
			case 0:
				texArray[texArrayIndex] = 0xFFFFFFFF;
				break;
			case 1:
				texArray[texArrayIndex] = 0x00FF00FF;
				break;
			case 2:
				texArray[texArrayIndex] = 0xBBBBBBFF;
				break;
			case 3:
				texArray[texArrayIndex] = 0x00FFFFFF;
				break;
			default:
				break;
			}
		}
	}
}
