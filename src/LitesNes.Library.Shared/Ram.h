#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Ram
{
public:

	Ram(std::string name);
	~Ram();

	void Render();
	void Add(uint8_t addition);
	void Subtract(uint8_t subtraction);

private:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData[1024*2];
	bool mIsAccumulator = false;
};
