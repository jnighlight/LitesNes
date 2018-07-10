#include "pch.h"
#include "OAMRegister.h"

OAMRegister::OAMRegister(std::string name, AddressRegister& registerToWrite, OAM& writeToRam)
	:Register(name)
	,mAddressRegister(registerToWrite)
	,mOAM(writeToRam)
{}

OAMRegister::~OAMRegister()
{}

void OAMRegister::Set(uint8_t inValue)
{
	mOAM.SetMemoryByLocation(mAddressRegister.GetAddress(), inValue);
	mAddressRegister.Increment();
}

void OAMRegister::Set(Register& otherReg)
{
	mOAM.SetMemoryByLocation(mAddressRegister.GetAddress(), otherReg.GetRegisterContents());
	mAddressRegister.Increment();
}

uint8_t OAMRegister::GetRegisterContents()
{
	uint8_t curMemContents = mOAM.GetMemoryByLocation(mAddressRegister.GetAddress());
	mAddressRegister.Increment();
	return curMemContents;
}
