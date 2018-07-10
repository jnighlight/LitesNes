#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class PatternTables
{
public:
	enum PatternTableSide
	{
		LEFT = 0,
		RIGHT
	};

	struct PatternTable
	{
		PatternTable(uint8_t* patternStartLocation);
		uint8_t lowPlane[8];
		uint8_t highPlane[8];
		uint8_t GetValueByIndex(uint8_t x, uint8_t y);
	};

	PatternTables(uint32_t charRomSize, char* chrRomData);
	~PatternTables();

	PatternTable getPatternByIndex(uint32_t index);
	PatternTable getPatternByIndex(uint32_t index, PatternTableSide side);

private:
	uint8_t * mTableData;
};
