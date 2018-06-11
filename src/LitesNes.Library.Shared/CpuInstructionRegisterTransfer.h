#pragma once
#include "CpuInstruction.h"
#include "Register.h"

class CpuInstructionRegisterTransfer : public CpuInstruction
{
public:
	CpuInstructionRegisterTransfer(Register& originRegister, Register& destRegister);
	virtual ~CpuInstructionRegisterTransfer();

	virtual void Execute();
private:
	Register & mOriginRegister;
	Register & mDestinationRegister;
};
