#include "pch.h"
#include <ctype.h>
#include "CpuInstructionRegisterTransfer.h"
#include "imgui.h"
#include "NesDebugger.h"

CpuInstructionRegisterTransfer::CpuInstructionRegisterTransfer(Register& originRegister, Register& destinationRegister)
	:mOriginRegister(originRegister), mDestinationRegister(destinationRegister)
{
}

CpuInstructionRegisterTransfer::~CpuInstructionRegisterTransfer()
{
}

void CpuInstructionRegisterTransfer::Execute()
{
	mDestinationRegister.Set(mOriginRegister);

	NesDebugger::sStatusReg.SetZeroFlag(mDestinationRegister.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(mDestinationRegister.IsNegative());
}
