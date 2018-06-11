#pragma once

#include "Register.h"

class StatusRegister : public Register
{
public:
	StatusRegister(std::string name);
	virtual ~StatusRegister();

	void Render();
	void SetZeroFlag(bool zeroFlag);
	bool GetZeroFlag();
	void SetNegativeFlag(bool negativeFlag);
	bool GetNegativeFlag();
};
