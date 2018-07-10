#include "pch.h"
#include "PPU.h"
#include <ctype.h>
#include <vector>
#include "Ram.h"
#include "imgui.h"
#include "NesDebugger.h"

Register PPU::PPUCTRL = Register("PPUCTRL");
Register PPU::PPUMASK = Register("PPUMASK");
Register PPU::PPUSTATUS = Register("PPUSTATUS");
Register PPU::OAMADDR = Register("OAMADDR");
Register PPU::OAMDATA = Register("OAMDATA");
Register PPU::PPUSCROLL = Register("PPUSCROLL");
Register PPU::PPUADDR = Register("PPUADDR");
Register PPU::PPUDATA = Register("PPUDATA");
Register PPU::OAMDMA = Register("OAMDMA");
Ram PPU::VRam = Ram("VRam");

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
	uint8_t drawX = 0;
	uint8_t drawY = 0;
	for (uint32_t j = 0; j < 16; ++j)
	{
		for (uint32_t i = 0; i < 32; ++i)
		{
			PatternTables::PatternTable curTable = mPatternTables.getPatternByIndex(i + (j * 32));
			for (uint8_t x = 0; x < 8; ++x)
			{
				for (uint8_t y = 0; y < 8; ++y)
				{
					uint32_t texArrayIndex = (drawX + x) + (256 * (drawY + y));
					uint8_t value = curTable.GetValueByIndex(x, y);
					switch (value)
					{
					case 0:
						texArray[texArrayIndex] = 0xFFFFFFFF;
						break;
					case 1:
						texArray[texArrayIndex] = 0x00FF00FF;
						break;
					case 2:
						texArray[texArrayIndex] = 0xAAAAAAFF;
						break;
					case 3:
						texArray[texArrayIndex] = 0xFF00FFFF;
						break;
					default:
						break;
					}
				}
			}
			drawX += 8;
		}
		drawY += 8;
	}
}
