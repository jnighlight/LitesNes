#include "pch.h"
#include "PPU.h"
#include <ctype.h>
#include <cmath>
#include <vector>
#include "Ram.h"
#include "imgui.h"
#include "NesDebugger.h"

PPU::PPUColor PPU::Colors[0x40] = {
	{84,84,84},    {0,30,116}, {8,16,144},  {48,0,136},  {68, 0, 100}, {92, 0, 48},  {84, 4, 0},    {60,24,0},  {32,42,0}, {8,58,0},   {0,64,0},  {0,60,0},   {0,50,60},   {0,0,0},{0,0,0},{0,0,0},
	{152,150,152}, {8,76,196}, {48,50,236}, {92,30,228}, {136,20,176}, {160,20,100}, {152, 34, 32}, {120,60,0}, {84,90,0}, {40,114,0}, {8,124,0}, {0,118,40}, {0,102,120}, {0,0,0},{0,0,0},{0,0,0},
	{236,238,236},  {76,154,236}, {120,124,236}, {176,98,236}, {228,84,236}, {236,88,180}, {236,106,100}, {212,136,32}, {160,170,0}, {116,196,0},  {76,208,32},  {56,204,108},  {56,180,204},  {60,60, 60},{0,0,0},{0,0,0},
	{236,238,236}, {168,204,236}, {188,188,236}, {212,178,236}, {236,174,236}, {236,174,212}, {236,180,176}, {228,196,144}, {204,210,120}, {180,222,120}, {168,226,144}, {152,226,180}, {160,214,228}, {160,162,160},{0,0,0},{0,0,0}
};
Ram PPU::VRam = Ram("VRam");
OAM PPU::sOAM;
Register PPU::PPUCTRL = Register("PPUCTRL");
Register PPU::PPUMASK = Register("PPUMASK");
Register PPU::PPUSTATUS = Register("PPUSTATUS", 0x80);
AddressRegister PPU::OAMADDR = AddressRegister("OAMADDR");
OAMRegister PPU::OAMDATA = OAMRegister("OAMDATA", PPU::OAMADDR, PPU::sOAM);
AddressRegister PPU::PPUSCROLL = AddressRegister("PPUSCROLL");
AddressRegister PPU::PPUADDR = AddressRegister("PPUADDR");
WritingRegister PPU::PPUDATA = WritingRegister("PPUDATA", PPU::PPUADDR, PPU::VRam);
OAMRegister PPU::OAMDMA = OAMRegister("OAMDMA", PPU::OAMADDR, PPU::sOAM); //TODO: set this sucker up to do full AOM setups
bool PPU::mRenderPatternTables = false;
bool PPU::mTableSide = false;

PPU::PPU(uint32_t chrRomSize, char* chrRomPtr)
	:mPatternTables(chrRomSize, chrRomPtr)
{
}

PPU::~PPU()
{
}

void PPU::Render(uint32_t* texArray)
{
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

/*
void PPU::GetPalette(uint16_t row, uint8_t column)
{
	uint16_t startingPoint = 0x23C0;
}*/

void PPU::DrawBackground(uint32_t lineNum, uint32_t* lineToDrawTo)
{
	uint16_t nametableLine = uint16_t(lineNum / 8);
	uint16_t nametableStartingIndex = ((256 / 8) * nametableLine) + 0x2000;
	//uint16_t attributeTableStartingIndex = nametableStartingIndex 
	uint16_t xOffset = 0;
	for (uint8_t i = 0; i < 32; ++i)
	{
		PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(VRam.GetMemoryByLocation(nametableStartingIndex + i), mTableSide ? PatternTables::LEFT : PatternTables::RIGHT);
		for (uint8_t x = 0; x < 8; ++x)
		{
			//uint8_t palette = GetPalette(nametableLine, i);
			//uint8_t* palette = ; get nametable palette (attribute table)
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
	uint8_t paletteNum = oamEntry.attributes & 0b00000011;
	uint8_t* palette = PPU::VRam.GetRamPtr() + 0x3F11 + ((4 * paletteNum));
	for (uint8_t i = 0; i < 8; ++i)
	{
		uint8_t color = curTable.GetValueByIndex(i, row);
		uint8_t* index = palette + (color-1);
		uint32_t finalColor = 0;
		switch (color)
		{
		case 0:
			continue;
			break;
		case 1:
			finalColor = PPU::Colors[*index].GetColor();
			break;
		case 2:
			finalColor = PPU::Colors[*index].GetColor();
			break;
		case 3:
			finalColor = PPU::Colors[*index].GetColor();
			break;
		default:
			break;
		}
		lineToDrawTo[oamEntry.xPos + i] = finalColor;
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
	uint16_t nameTableIndex = 0x2000;

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
