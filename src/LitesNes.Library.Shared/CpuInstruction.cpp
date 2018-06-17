#include "pch.h"
#include <ctype.h>
#include "CpuInstruction.h"
#include "imgui.h"
#include "NesDebugger.h"

std::map<uint8_t, CpuInstruction::OperationDescription> CpuInstruction::sOperations = {

	{0x00, {"NOP", 1, 1, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::NoOperation}},
	{0xF8, {"SED", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetDecimalFlag}},
	{0x78, {"SEI", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetInterruptFlag}},

	{0x84, {"STY", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,				NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x94, {"STY", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,	NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8C, {"STY", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr, 				NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	{0x86, {"STX", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,				NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x96, {"STX", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mYReg,	NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8E, {"STX", 2, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr, 				NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	{0x85, {"STA", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,				NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x95, {"STA", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8D, {"STA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,				NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x9D, {"STA", 3, 5, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x99, {"STA", 3, 5, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x81, {"STA", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x91, {"STA", 2, 6, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	{0x8A, {"TXA", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,				NesDebugger::mXReg, NesDebugger::mAReg, CpuInstruction::TransferDataInstruction}},
	{0x98, {"TYA", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mYReg, NesDebugger::mAReg, CpuInstruction::TransferDataInstruction}},
	{0x9A, {"TXS", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mXReg, NesDebugger::mSPReg, CpuInstruction::TransferDataInstruction}},
	{0xA8, {"TAY", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mAReg, NesDebugger::mYReg, CpuInstruction::TransferDataInstruction}},
	{0xAA, {"TAX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mAReg, NesDebugger::mXReg, CpuInstruction::TransferDataInstruction}},
	{0xBA, {"TSX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 				NesDebugger::mSPReg, NesDebugger::mXReg, CpuInstruction::TransferDataInstruction}}
};

void CpuInstruction::TransferDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	assert(firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed);
	assert(memoryModRegister == nullptr);

	toDefault.SetData(fromDefault.GetData());

	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::StoreDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	toDefault.SetData(fromDefault.GetData(), firstArgument.GetMemoryLocation());
}

void CpuInstruction::SetInterruptFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetInterruptDisableFlag(true);
}

void CpuInstruction::NoOperation(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
}

void CpuInstruction::SetDecimalFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	//TODO: but the RICOH chip the NES uses doesn't actually have this feature, so...meh
	//NesDebugger::sStatusReg.SetDecimalFlag(true);
}

void CpuInstruction::SetCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetCarryFlag(true);
}

CpuInstruction::CpuInstruction()
	:mDescription(CpuInstruction::sOperations.find(0)->second)
{
}

CpuInstruction::CpuInstruction(uint8_t opcode, GenericInstructionTarget firstTarget)
	:mOpCode(opcode), mFirstArgument(firstTarget), mDescription(CpuInstruction::sOperations.find(mOpCode)->second)
{
}

CpuInstruction::CpuInstruction(uint8_t opcode, const OperationDescription& description, uint8_t* argumentLocation)
	:mOpCode(opcode), mDescription(description)
{
	uint16_t inArgData = 0;
	if (mDescription.mBytes == 2) {
		inArgData = argumentLocation[1];
	} else if (mDescription.mBytes == 3) {
		inArgData = argumentLocation[2];
		inArgData = inArgData << 8;
		inArgData |= argumentLocation[1];
	}
	mFirstArgument.SetTargetType(description.mFirstArgumentType);
	mFirstArgument.SetLiteralData(inArgData);
}

void CpuInstruction::Execute()
{
	auto operationSearchResult = sOperations.find(mOpCode);
	if (operationSearchResult == sOperations.end()) {
		throw(std::exception("Executing not-found command"));
	}
	CpuInstruction::OperationDescription& description = operationSearchResult->second;
	description.mExecuteFunction(mFirstArgument, description.mMemoryModReg, description.mDefaultFromTarget, description.mDefaultToTarget);
}

CpuInstruction::~CpuInstruction()
{
}

std::string CpuInstruction::GetOpcodeName(uint8_t opcode)
{
	return sOperations[opcode].mOpcodeName;
}

std::string CpuInstruction::GetArgumentDescription()
{
	char fullAddress[5];
	switch (mDescription.mFirstArgumentType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		return std::string("");
		break;
	case GenericInstructionTarget::ERegister:
		throw(std::exception("Register wat?"));
		break;
	case GenericInstructionTarget::ERam:
		NesDebugger::PopulateCharBufferWithHex(fullAddress, mFirstArgument.GetLiteralData());
		break;
	case GenericInstructionTarget::EAbsolute:
		NesDebugger::PopulateCharBufferWithHex(fullAddress, mFirstArgument.GetLiteralData());
		break;
	case GenericInstructionTarget::EZeroPage:
		NesDebugger::PopulateCharBufferWithHex(fullAddress, uint8_t(mFirstArgument.GetLiteralData()));
		break;
	case GenericInstructionTarget::EImmediate:
		throw(std::exception("Register Modifying Immediate, that's not rite..."));
		break;
	case GenericInstructionTarget::EIndexedIndirect:
		//TODO: Get zero page val, add X to it, then point to the resultant memory
		break;
	case GenericInstructionTarget::EIndirectIndexed:
		//TODO: Get zero page val, store data at location, add Y to it, then point to the resultant memory
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	return std::string(fullAddress);
}

std::string CpuInstruction::GetRegOffsetString()
{
	if (mDescription.mMemoryModReg != nullptr)
	{
		std::string ret(",");
		ret.append(mDescription.mMemoryModReg->GetName());
		return ret;
	}
	return std::string("");
}
