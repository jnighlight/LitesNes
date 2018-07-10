#include "pch.h"
#include "WritingRegister.h"

WritingRegister::WritingRegister(std::string name, AddressRegister& registerToWrite, Ram& writeToRam)
	:Register(name)
	,mAddressRegister(registerToWrite)
	,mRam(writeToRam)
{}

WritingRegister::~WritingRegister()
{}

void WritingRegister::Set(uint8_t inValue)
{
	mRam.SetMemoryByLocation(mAddressRegister.GetAddress(), inValue);
	mAddressRegister.Increment();
}

void WritingRegister::Set(Register& otherReg)
{
	mRam.SetMemoryByLocation(mAddressRegister.GetAddress(), otherReg.GetRegisterContents());
	mAddressRegister.Increment();
}

uint8_t WritingRegister::GetRegisterContents()
{
	uint8_t curMemContents = mRam.GetMemoryByLocation(mAddressRegister.GetAddress());
	mAddressRegister.Increment();
	return curMemContents;
}
