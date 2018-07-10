#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "AddressRegister.h"
#include "OAM.h"

class OAMRegister : public Register
{
public:

	OAMRegister(std::string name, AddressRegister& registerToWrite, OAM& writeToRam);
	virtual ~OAMRegister();

	virtual void Set(uint8_t inValue) override;
	virtual void Set(Register& otherReg) override;
	virtual uint8_t GetRegisterContents() override;

protected:
	AddressRegister& mAddressRegister;
	OAM& mOAM;
};
