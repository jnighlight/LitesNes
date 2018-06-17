#pragma once
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Rom
{
public:

	Rom();
	Rom(std::string name);
	~Rom();

private:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData[1024*2];
	bool mIsAccumulator = false;
};
