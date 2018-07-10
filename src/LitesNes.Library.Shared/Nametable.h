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
	uint8_t mData[1024*2];
};
