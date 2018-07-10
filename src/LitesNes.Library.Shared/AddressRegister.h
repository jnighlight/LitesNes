#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "Register.h"

class AddressRegister : public Register
{
public:

	AddressRegister();
	AddressRegister(std::string name);
	virtual ~AddressRegister();

	virtual void Set(uint8_t inValue) override;
	virtual void Set(Register& otherReg) override;
	void Increment() { ++mAddress; };
	virtual uint8_t GetRegisterContents() override { throw std::exception("Accessing register in wrong way"); };
	uint16_t GetAddress() { return mAddress; };

protected:
	uint16_t mAddress = 0;
};
