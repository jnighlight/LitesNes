#include "pch.h"
#include "PPU.h"
#include <ctype.h>
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
}

PPU::~PPU()
{
}

void PPU::Render()
{
}

void PPU::RenderLine(uint32_t lineNum)
{
	UNREFERENCED_PARAMETER(lineNum);
	//TODO: Draw background from VRAM (nametables and attribute tables)
	/*
	std::vector<OAM::OAMEntry> oamEntries = mOAM.GetCollidingSprites(lineNum);
	if (oamEntries.size() > 8) {
		//set sprite overflow flag
	}
	for( auto oamEntry : oamEntries)
	{
		Sprite sprite = mPatternTables.getPattern(oamEntry);
		DrawSpriteLine(sprite, oamEntry);
	}
	*/
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
