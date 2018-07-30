#include "pch.h"
#include "OAMRegister.h"
#include "PPU.h"

OAMRegister::OAMRegister(std::string name, AddressRegister& registerToWrite, OAM& writeToRam)
	:Register(name)
	,mAddressRegister(registerToWrite)
	,mOAM(writeToRam)
{}

OAMRegister::~OAMRegister()
{}

void OAMRegister::Set(uint8_t inValue)
{
	if (mName == "OAMDMA")
	{
		PPU::sOAM.LoadAllOAM(inValue);
		return;
	}
	mOAM.SetMemoryByLocation(mAddressRegister.GetAddress(), inValue);
	mAddressRegister.Increment();
}

void OAMRegister::Set(Register& otherReg)
{
	if (mName == "OAMDMA")
	{
		PPU::sOAM.LoadAllOAM(otherReg.GetRegisterContents());
		return;
	}
	mOAM.SetMemoryByLocation(mAddressRegister.GetAddress(), otherReg.GetRegisterContents());
	mAddressRegister.Increment();
}

uint8_t OAMRegister::GetRegisterContents()
{
	uint8_t curMemContents = mOAM.GetMemoryByLocation(mAddressRegister.GetAddress());
	mAddressRegister.Increment();
	return curMemContents;
}
