#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class Register
{
public:

	Register();
	Register(std::string name);
	virtual ~Register();

	virtual void Render();
	void Add(uint8_t addition);
	void Subtract(uint8_t subtraction);
	virtual void Set(uint8_t inValue);
	virtual void Set(Register& otherReg);
	bool IsZero() const;
	bool IsNegative() const;
	virtual uint8_t GetRegisterContents() { return mData; };
	std::string GetName() { return mName; };

protected:
	std::string mName;
	std::string mHiddenName;
	uint8_t mData;
	bool mIsAccumulator = false;
};
