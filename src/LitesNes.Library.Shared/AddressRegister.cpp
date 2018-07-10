#include "pch.h"
#include "AddressRegister.h"

AddressRegister::AddressRegister()
	:Register("DefaultRegName")
{}

AddressRegister::AddressRegister(std::string name)
	:Register(name)
{}

AddressRegister::~AddressRegister()
{}

void AddressRegister::Set(uint8_t inValue)
{
	mAddress = mAddress << 8;
	mAddress |= inValue;
}

void AddressRegister::Set(Register& otherReg)
{
	mAddress = mAddress << 8;
	mAddress |= otherReg.GetRegisterContents();
}
