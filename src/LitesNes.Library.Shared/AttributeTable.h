#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class AttributeTable
{
public:

	AttributeTable();
	AttributeTable(std::string name);
	~AttributeTable();

	void Render();
	void Add(uint8_t addition);
	void Subtract(uint8_t subtraction);
	void SetMemoryByLocation(uint16_t memoryLocation, uint8_t data);
	uint8_t GetMemoryByLocation(uint16_t memoryLocation);

private:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData[1024*2];
	bool mIsAccumulator = false;
};
