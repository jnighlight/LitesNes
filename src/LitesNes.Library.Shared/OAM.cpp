#include "pch.h"
#include "OAM.h"
#include <ctype.h>
#include "OAM.h"
#include "imgui.h"
#include "NesDebugger.h"

OAM::OAM()
{}

OAM::OAM(std::string name)
{
}

OAM::~OAM()
{
}

std::vector<OAM::OAMEntry> OAM::GetCollidingSprites(uint32_t lineNum)
{
	uint32_t spriteRange = lineNum + 8; //Sprites are generally 8 pixels tall
	std::vector<OAM::OAMEntry> collisionSprites(8); //Assume that we _usually_ won't have more than 8 sprites on a line
	for (auto oam : mEntries)
	{
		if (oam.yPos >= lineNum && oam.yPos <= spriteRange)
		{
			collisionSprites.push_back(oam);
		}
	}
	return collisionSprites;
}

void OAM::LoadAllOAM(uint8_t* startLoadLocation)
{
	for (uint32_t i = 0; i < 64; ++i)
	{
		mEntries[i].yPos = *startLoadLocation;
		++startLoadLocation;
		mEntries[i].tileIndex = *startLoadLocation;
		++startLoadLocation;
		mEntries[i].attributes = *startLoadLocation;
		++startLoadLocation;
		mEntries[i].xPos = *startLoadLocation;
		++startLoadLocation;
	}
}

void OAM::SetMemoryByLocation(uint16_t address, uint8_t value)
{
	assert(address <= 64 * 4);
	uint8_t* entryDataByByte = reinterpret_cast<uint8_t*>(mEntries);
	entryDataByByte[address] = value;
}

uint8_t OAM::GetMemoryByLocation(uint16_t address)
{
	assert(address <= 64 * 4);
	uint8_t* entryDataByByte = reinterpret_cast<uint8_t*>(mEntries);
	return entryDataByByte[address];
}
