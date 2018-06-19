#include "pch.h"
#include <ctype.h>
#include "CpuInstruction.h"
#include "imgui.h"
#include "NesDebugger.h"

std::map<uint8_t, CpuInstruction::OperationDescription> CpuInstruction::sOperations = {

	//Noop
	{0x00, {"NOP", 1, 1, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::NoOperation}},

	//Flag Sets
	{0xF8, {"SED", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetDecimalFlag}},
	{0x78, {"SEI", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetInterruptFlag}},

	//Loading Registers from memory
	{0xA2, {"LDX", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mXReg, CpuInstruction::LoadDataInstruction}},
	{0xA6, {"LDX", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mXReg, CpuInstruction::LoadDataInstruction}},
	{0xB6, {"LDX", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mXReg, CpuInstruction::LoadDataInstruction}},
	{0xAE, {"LDX", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mXReg, CpuInstruction::LoadDataInstruction}},
	{0xBE, {"LDX", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mXReg, CpuInstruction::LoadDataInstruction}},

	{0xA0, {"LDY", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mYReg, CpuInstruction::LoadDataInstruction}},
	{0xA4, {"LDY", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mYReg, CpuInstruction::LoadDataInstruction}},
	{0xB4, {"LDY", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mYReg, CpuInstruction::LoadDataInstruction}},
	{0xAC, {"LDY", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mYReg, CpuInstruction::LoadDataInstruction}},
	{0xBC, {"LDY", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mYReg, CpuInstruction::LoadDataInstruction}},

	{0xA9, {"LDA", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xA5, {"LDA", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xB5, {"LDA", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xAD, {"LDA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xBD, {"LDA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xB9, {"LDA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xA1, {"LDA", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},
	{0xB1, {"LDA", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::LoadDataInstruction}},

	//Storing Registers into memory
	{0x84, {"STY", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x94, {"STY", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8C, {"STY", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr, 						NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	{0x86, {"STX", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x96, {"STX", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mYReg,			NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8E, {"STX", 2, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr, 						NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	{0x85, {"STA", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x95, {"STA", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x8D, {"STA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x9D, {"STA", 3, 5, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x99, {"STA", 3, 5, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x81, {"STA", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},
	{0x91, {"STA", 2, 6, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

	//Register Transfers
	{0xAA, {"TAX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg, NesDebugger::mXReg, CpuInstruction::TransferRegisterInstruction}},
	{0xA8, {"TAY", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg, NesDebugger::mYReg, CpuInstruction::TransferRegisterInstruction}},
	{0x8A, {"TXA", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mXReg, NesDebugger::mAReg, CpuInstruction::TransferRegisterInstruction}},
	{0x98, {"TYA", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mYReg, NesDebugger::mAReg, CpuInstruction::TransferRegisterInstruction}},
	{0xBA, {"TSX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mSPReg, NesDebugger::mXReg, CpuInstruction::TransferRegisterInstruction}},
	{0x9A, {"TXS", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mXReg, NesDebugger::mSPReg, CpuInstruction::TransferRegisterInstruction}},

	//Logical Operations
	{0x29, {"AND", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x25, {"AND", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x35, {"AND", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x2D, {"AND", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x3D, {"AND", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x39, {"AND", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x21, {"AND", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},
	{0x31, {"AND", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AndOperationInstruction}},

	{0x49, {"EOR", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x45, {"EOR", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x55, {"EOR", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x4D, {"EOR", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x5D, {"EOR", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x59, {"EOR", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x41, {"EOR", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},
	{0x51, {"EOR", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::EorOperationInstruction}},

	{0x09, {"ORA", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x05, {"ORA", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x15, {"ORA", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x0D, {"ORA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x1D, {"ORA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x19, {"ORA", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x01, {"ORA", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},
	{0x11, {"ORA", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::OrOperationInstruction}},

	{0x24, {"BIT", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BitTestInstruction}},
	{0x2C, {"BIT", 2, 5, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BitTestInstruction}},
};

void CpuInstruction::BitTestInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);

	assert(memoryModRegister == nullptr);

	uint8_t andedByte = firstArgument.GetData();
	andedByte &= toDefault.GetData();

	NesDebugger::sStatusReg.SetZeroFlag(andedByte == 0);
	NesDebugger::sStatusReg.SetNegativeFlag(andedByte & 0b10000000); //This is kinda weird, but it's what the spec says
	NesDebugger::sStatusReg.SetOverflowFlag(andedByte & 0b01000000); //This too, but whatever
}

void CpuInstruction::OrOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);
	if (memoryModRegister != nullptr) {
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	uint8_t andByteFromMemory = firstArgument.GetData();
	uint8_t andByteFromReg = toDefault.GetData();
	toDefault.SetData(andByteFromReg | andByteFromMemory);
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::EorOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);
	if (memoryModRegister != nullptr) {
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	uint8_t andByteFromMemory = firstArgument.GetData();
	toDefault.SetData(toDefault.GetData() ^ andByteFromMemory);
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::AndOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);
	if (memoryModRegister != nullptr) {
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	uint8_t andByteFromMemory = firstArgument.GetData();
	toDefault.SetData(toDefault.GetData() & andByteFromMemory);
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::TransferRegisterInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	assert(firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed);
	assert(memoryModRegister == nullptr);
	toDefault.SetData(fromDefault.GetData());
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::StoreDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	if (memoryModRegister != nullptr) {
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	toDefault.SetData(fromDefault.GetData(), firstArgument.GetMemoryLocation());
}

void CpuInstruction::LoadDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);
	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	toDefault.SetData(firstArgument.GetData(), firstArgument.GetMemoryLocation());
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
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
	if (description.mFirstArgumentType != GenericInstructionTarget::ERegister) {
		mFirstArgument.SetDataSource(NesDebugger::mRam, description.mFirstArgumentType);
	} else if (description.mMemoryModReg != nullptr) {
		mFirstArgument.SetDataSource(*description.mMemoryModReg);
	}
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

void CpuInstruction::GetArgumentDescription(std::string& stringToAppendTo)
{
	char fullAddress[5];
	switch (mDescription.mFirstArgumentType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		return;
		break;
	case GenericInstructionTarget::ERegister:
		throw(std::exception("Register wat?"));
		break;
	case GenericInstructionTarget::ERam:
	case GenericInstructionTarget::EAbsolute:
		stringToAppendTo.append("$");
		NesDebugger::PopulateCharBufferWithHex(fullAddress, mFirstArgument.GetLiteralData());
		break;
	case GenericInstructionTarget::EZeroPage:
		stringToAppendTo.append("$");
		NesDebugger::PopulateCharBufferWithHex(fullAddress, uint8_t(mFirstArgument.GetLiteralData()));
		break;
	case GenericInstructionTarget::EImmediate:
		stringToAppendTo.append("#");
		NesDebugger::PopulateCharBufferWithHex(fullAddress, uint8_t(mFirstArgument.GetLiteralData()));
		break;
	case GenericInstructionTarget::EIndexedIndirect:
	case GenericInstructionTarget::EIndirectIndexed:
		stringToAppendTo.append("(");
		NesDebugger::PopulateCharBufferWithHex(fullAddress, mFirstArgument.GetLiteralData());
		break;
	case GenericInstructionTarget::EMax:
		throw(std::exception("Max Instruction flying around"));
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	stringToAppendTo.append(fullAddress);
	if (mDescription.mFirstArgumentType == GenericInstructionTarget::EIndirectIndexed)
	{
		stringToAppendTo.append(")");
	}
	GetRegOffsetString(stringToAppendTo);
	if (mDescription.mFirstArgumentType == GenericInstructionTarget::EIndexedIndirect)
	{
		stringToAppendTo.append(")");
	}
}

void CpuInstruction::GetRegOffsetString(std::string& stringToAppendTo)
{
	if (mDescription.mMemoryModReg != nullptr)
	{
		stringToAppendTo.append(",");
		stringToAppendTo.append(mDescription.mMemoryModReg->GetName());
	}
}
