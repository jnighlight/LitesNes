#include "PatternTables.h"
#include "pch.h"
#include <ctype.h>
#include "PatternTables.h"
#include "imgui.h"
#include "NesDebugger.h"

PatternTables::PatternTables(uint32_t charRomSize, char* chrRomData)
{
	mTableData = new uint8_t[charRomSize];
	memcpy(mTableData, chrRomData, charRomSize);
}

PatternTables::~PatternTables()
{
	delete mTableData;
}

PatternTables::PatternTable PatternTables::getPatternByIndex(uint32_t index)
{
	assert(index <= 512);
	assert(index >= 0);
	uint32_t tableIndex = index;
	tableIndex = tableIndex << 4;
	uint8_t* tableLocation = mTableData + tableIndex;
	return PatternTable(tableLocation);
}

PatternTables::PatternTable PatternTables::getPatternByIndex(uint32_t index, PatternTableSide side)
{
	assert(index <= 256);
	assert(index >= 0);
	uint16_t tableIndex = 0;
	if (side == RIGHT)
	{
		tableIndex |= 0x0100; //we're gonna left shift it one nybble
	}
	tableIndex |= index;
	tableIndex = tableIndex << 4;
	uint8_t* tableLocation = mTableData + tableIndex;
	return PatternTable(tableLocation);
}

/*
---------------------------------------
			PatternTable
---------------------------------------
*/
PatternTables::PatternTable::PatternTable(uint8_t* patternStartLocation)
{
	for (uint32_t i = 0; i < 8; ++i)
	{
		lowPlane[i] = patternStartLocation[i];
	}
	patternStartLocation += 8;
	for (uint32_t i = 0; i < 8; ++i)
	{
		highPlane[i] = patternStartLocation[i];
	}
}

uint8_t PatternTables::PatternTable::GetValueByIndex(uint8_t x, uint8_t y)
{
	assert(x <= 7);
	assert(y <= 7);
	uint8_t highByte = highPlane[y];
	highByte = highByte & (0b10000000 >> x);
	if (highByte > 0)
	{
		highByte = 1;
	}

	uint8_t lowByte = lowPlane[y];
	uint8_t toAnd = (uint8_t(0b10000000) >> x);
	lowByte = lowByte & toAnd;
	if (lowByte > 0)
	{
		lowByte = 1;
	}
	return (highByte << 1) | lowByte;
}
