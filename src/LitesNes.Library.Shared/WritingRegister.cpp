#include "pch.h"
#include "WritingRegister.h"
#include "PPU.h"

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
	if (&mAddressRegister == &PPU::PPUADDR && (PPU::PPUCTRL.GetRegisterContents() & 0b00000100))
	{
		mAddressRegister.LineIncrement();
		return;
	}
	mAddressRegister.Increment();
}

void WritingRegister::Set(Register& otherReg)
{
	mRam.SetMemoryByLocation(mAddressRegister.GetAddress(), otherReg.GetRegisterContents());
	if (&mAddressRegister == &PPU::PPUADDR && (PPU::PPUCTRL.GetRegisterContents() & 0b00000100))
	{
		mAddressRegister.LineIncrement();
		return;
	}
	mAddressRegister.Increment();
}

uint8_t WritingRegister::GetRegisterContents()
{
	uint8_t curMemContents = mRam.GetMemoryByLocation(mAddressRegister.GetAddress());
	mAddressRegister.Increment();
	return curMemContents;
}
