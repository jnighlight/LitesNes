#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Nametable
{
public:

	Nametable();
	Nametable(std::string name);
	~Nametable();

private:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData[1024*2];
	bool mIsAccumulator = false;
};
