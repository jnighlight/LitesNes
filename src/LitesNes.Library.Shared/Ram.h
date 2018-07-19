#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Ram
{
public:

	Ram();
	Ram(std::string name);
	~Ram();

	void Render();
	void SetMemoryByLocation(uint16_t memoryLocation, uint8_t data);
	uint8_t GetMemoryByLocation(uint16_t memoryLocation);
	void UpdateCachedRamString();
	std::string int_to_hex(int i);

private:
	uint32_t rerenderCount = 0;
	std::string mName;
	std::string mHiddenName;
	std::vector<std::string> mCachedRamStrings;
	uint8_t mData[0xFFFF];
	bool mIsAccumulator = false;
};
