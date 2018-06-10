#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Register
{
public:

	Register(std::string name);
	~Register();

	void Render();
	void Add(uint8_t addition);
	void Subtract(uint8_t subtraction);

private:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData;
	bool mIsAccumulator = false;
};
