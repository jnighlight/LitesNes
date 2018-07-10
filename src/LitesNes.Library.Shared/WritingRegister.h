#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "AddressRegister.h"
#include "Ram.h"

class WritingRegister : public Register
{
public:

	WritingRegister(std::string name, AddressRegister& registerToWrite, Ram& writeToRam);
	virtual ~WritingRegister();

	virtual void Set(uint8_t inValue) override;
	virtual void Set(Register& otherReg) override;
	virtual uint8_t GetRegisterContents() override;

protected:
	AddressRegister& mAddressRegister;
	Ram& mRam;
};
