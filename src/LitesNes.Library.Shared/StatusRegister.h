#pragma once

#include "Register.h"

class StatusRegister : public Register
{
public:
	StatusRegister(std::string name);
	virtual ~StatusRegister();

	virtual void Render();
	void SetZeroFlag(bool zeroFlag);
	bool GetZeroFlag();
	void SetNegativeFlag(bool negativeFlag);
	bool GetNegativeFlag();
	void SetInterruptDisableFlag(bool interruptDisableFlag);
	bool GetInterruptDisableFlag();
	void SetCarryFlag(bool carryDisableFlag);
	bool GetCarryFlag();
	void SetOverflowFlag(bool overflowFlag);
	bool GetOverflowFlag();
	void SetDecimalFlag(bool decimalFlag);
	bool GetDecimalFlag();
};
