#include "CpuInstruction.h"
#include "pch.h"
#include <ctype.h>
#include "CpuInstruction.h"
#include "ByteHelper.h"
#include "imgui.h"
#include "NesDebugger.h"

std::map<uint8_t, CpuInstruction::OperationDescription> CpuInstruction::sOperations = {

	//Noop
	{0x00, {"NOP", 1, 1, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::NoOperation}},

	//Flag Sets
	{0xF8, {"SED", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetDecimalFlag}},
	{0x78, {"SEI", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetInterruptFlag}},
	{0x38, {"SEC", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetCarryFlag}},
	{0x38, {"SEC", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::SetCarryFlag}},

	//Flag Clears
	{0x18, {"CLC", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::ClearCarryFlag}},
	{0xD8, {"CLD", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::ClearDecimalFlag}},
	{0x58, {"CLI", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::ClearInterruptFlag}},
	{0xB8, {"CLV", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr, 						NesDebugger::mXReg, NesDebugger::mXReg, CpuInstruction::ClearOverflowFlag}},

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
	{0x8E, {"STX", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr, 						NesDebugger::mXReg, NesDebugger::mRam, CpuInstruction::StoreDataInstruction}},

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
	{0x9A, {"TXS", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mXReg, NesDebugger::mSPReg, CpuInstruction::TransferRegisterInstructionNoFlag}},

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
	{0x2C, {"BIT", 3, 5, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BitTestInstruction}},

	//Jump Operations
	{0x4C, {"JMP", 3, 3, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::JumpInstruction}},
	{0x6C, {"JMP", 3, 5, GenericInstructionTarget::TargetType::EIndirect, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::JumpIndirect}},

	{0xF0, {"BEQ", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfEqualInstruction}},
	{0xD0, {"BNE", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfNotEqualInstruction}},

	{0x90, {"BCC", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfCarryClear}},
	{0xB0, {"BCS", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfCarrySet}},

	{0x30, {"BMI", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfMinus}},
	{0x10, {"BPL", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfPositive}},

	{0x50, {"BVC", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfOverflowClear}},
	{0x70, {"BVS", 2, 2, GenericInstructionTarget::TargetType::ERelative, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::BranchIfOverflowSet}},

	//Subroutine Operations
	{0x20, {"JSR", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::JumpToSubroutine}},
	{0x60, {"RTS", 1, 6, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::JumpFromSubroutine}},
	{0x40, {"RTI", 1, 6, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::ReturnFromInterrupt}},

	//Stack Operations
	{0x48, {"PHA", 1, 3, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::PushByteToStack}},
	{0x08, {"PHP", 1, 3, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::sStatusReg, NesDebugger::mRam, CpuInstruction::PushByteToStack}},
	{0x68, {"PLA", 1, 4, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::PopByteFromStackNegZero}},
	{0x28, {"PLP", 1, 4, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mRam, NesDebugger::sStatusReg, CpuInstruction::PopByteFromStack}},

	//Shift Operations
	{0x0A, {"ASL", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::ShiftLeft}},
	{0x06, {"ASL", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftLeft}},
	{0x16, {"ASL", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftLeft}},
	{0x0E, {"ASL", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftLeft}},
	{0x1E, {"ASL", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftLeft}},

	{0x4A, {"LSR", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::ShiftRight}},
	{0x46, {"LSR", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftRight}},
	{0x56, {"LSR", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftRight}},
	{0x4E, {"LSR", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftRight}},
	{0x5E, {"LSR", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::ShiftRight}},

	{0x2A, {"ROL", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::RotateLeft}},
	{0x26, {"ROL", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateLeft}},
	{0x36, {"ROL", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateLeft}},
	{0x2E, {"ROL", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateLeft}},
	{0x3E, {"ROL", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateLeft}},

	{0x6A, {"ROR", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::RotateRight}},
	{0x66, {"ROR", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateRight}},
	{0x76, {"ROR", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateRight}},
	{0x6E, {"ROR", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateRight}},
	{0x7E, {"ROR", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::RotateRight}},

	//Inc/Dec Operations
	{0xE6, {"INC", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::Increment}},
	{0xF6, {"INC", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Increment}},
	{0xEE, {"INC", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Increment}},
	{0xFE, {"INC", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Increment}},

	{0xE8, {"INX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mXReg,NesDebugger::mRam, CpuInstruction::Increment}},
	{0xC8, {"INY", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::Increment}},

	{0xC6, {"DEC", 2, 5, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg,NesDebugger::mRam, CpuInstruction::Decrement}},
	{0xD6, {"DEC", 2, 6, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Decrement}},
	{0xCE, {"DEC", 3, 6, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Decrement}},
	{0xDE, {"DEC", 3, 7, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mRam, CpuInstruction::Decrement}},

	{0xCA, {"DEX", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mXReg,NesDebugger::mRam, CpuInstruction::Decrement}},
	{0x88, {"DEY", 1, 2, GenericInstructionTarget::TargetType::ENotUsed, nullptr,						NesDebugger::mYReg, NesDebugger::mRam, CpuInstruction::Decrement}},

	//Arithmetic Operations
	{0x69, {"ADC", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x65, {"ADC", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x75, {"ADC", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x6D, {"ADC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x7D, {"ADC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x79, {"ADC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x61, {"ADC", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},
	{0x71, {"ADC", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::AddWithCarryInstruction}},

	{0xE9, {"SBC", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xE5, {"SBC", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xF5, {"SBC", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xED, {"SBC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xFD, {"SBC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xF9, {"SBC", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xE1, {"SBC", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},
	{0xF1, {"SBC", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mRam, NesDebugger::mAReg, CpuInstruction::SubtractWithCarryInstruction}},

	{0xC9, {"CMP", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xC5, {"CMP", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xD5, {"CMP", 2, 4, GenericInstructionTarget::TargetType::EZeroPage, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xCD, {"CMP", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xDD, {"CMP", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mXReg,			NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xD9, {"CMP", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, &NesDebugger::mYReg,			NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xC1, {"CMP", 2, 6, GenericInstructionTarget::TargetType::EIndexedIndirect, &NesDebugger::mXReg,	NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xD1, {"CMP", 2, 5, GenericInstructionTarget::TargetType::EIndirectIndexed, &NesDebugger::mYReg,	NesDebugger::mAReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},

	{0xE0, {"CPX", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mXReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xE4, {"CPX", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mXReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xEC, {"CPX", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mXReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},

	{0xC0, {"CPY", 2, 2, GenericInstructionTarget::TargetType::EImmediate, nullptr,						NesDebugger::mYReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xC4, {"CPY", 2, 3, GenericInstructionTarget::TargetType::EZeroPage, nullptr,						NesDebugger::mYReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
	{0xCC, {"CPY", 3, 4, GenericInstructionTarget::TargetType::EAbsolute, nullptr,						NesDebugger::mYReg, NesDebugger::mAReg, CpuInstruction::CompareWithRegister}},
};

void CpuInstruction::CompareWithRegister(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	uint8_t regVal = fromDefault.GetData();
	uint8_t memVal = firstArgument.GetData();
	int8_t total = regVal - memVal;

	NesDebugger::sStatusReg.SetCarryFlag(regVal >= memVal);
	NesDebugger::sStatusReg.SetZeroFlag(regVal == memVal);
	NesDebugger::sStatusReg.SetNegativeFlag(total < 0);
}

void CpuInstruction::SubtractWithCarryInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);

	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	CpuInstruction::AddWithCarryLogic(toDefault.GetData(), ~(firstArgument.GetData()), toDefault); //Nifty little cheat!
}

void CpuInstruction::AddWithCarryInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);

	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	AddWithCarryLogic(toDefault.GetData(), firstArgument.GetData(), toDefault);
}

void CpuInstruction::AddWithCarryLogic(uint8_t acc, uint8_t arg, GenericInstructionTarget& spotToStore)
{
	int8_t carryModifier = NesDebugger::sStatusReg.GetCarryFlag() ? 1 : 0;
	uint16_t overflowCheckUnsigned = uint16_t(acc) + uint16_t(arg) + carryModifier;
	uint8_t sum = acc + arg + carryModifier;

	NesDebugger::sStatusReg.SetCarryFlag(overflowCheckUnsigned > 0xFF);
	bool overFlow = ~(acc ^ arg) & (acc ^ overflowCheckUnsigned) & 0x80;
	NesDebugger::sStatusReg.SetOverflowFlag(overFlow);
	NesDebugger::sStatusReg.SetZeroFlag(ByteHelper::IsZero(sum));
	NesDebugger::sStatusReg.SetNegativeFlag(ByteHelper::IsNegative(sum));

	spotToStore.SetData(sum);
}

void CpuInstruction::Decrement(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(int8_t(fromDefault.GetData()) - 1);
		NesDebugger::sStatusReg.SetZeroFlag(fromDefault.IsZero());
		NesDebugger::sStatusReg.SetNegativeFlag(fromDefault.IsNegative());
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		firstArgument.SetData(int8_t(firstArgument.GetData()) - 1);
		NesDebugger::sStatusReg.SetZeroFlag(firstArgument.IsZero());
		NesDebugger::sStatusReg.SetNegativeFlag(firstArgument.IsNegative());
	}
}

void CpuInstruction::Increment(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(int8_t(fromDefault.GetData()) + 1);
		NesDebugger::sStatusReg.SetZeroFlag(fromDefault.IsZero());
		NesDebugger::sStatusReg.SetNegativeFlag(fromDefault.IsNegative());
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		firstArgument.SetData(int8_t(firstArgument.GetData()) + 1);
		NesDebugger::sStatusReg.SetZeroFlag(firstArgument.IsZero());
		NesDebugger::sStatusReg.SetNegativeFlag(firstArgument.IsNegative());
	}
}

void CpuInstruction::RotateRight(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	uint8_t byteToShift = 0;
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		byteToShift = fromDefault.GetData();
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		byteToShift = firstArgument.GetData();
	}
	bool newBit7 = NesDebugger::sStatusReg.GetCarryFlag();
	bool carryFlag = byteToShift & 0b00000001;
	byteToShift = byteToShift >> 1;
	if (newBit7) {
		byteToShift |= 0b10000000;
	}
	bool negFlag = byteToShift & 0b10000000;

	NesDebugger::sStatusReg.SetZeroFlag(byteToShift == 0);
	NesDebugger::sStatusReg.SetNegativeFlag(negFlag);
	NesDebugger::sStatusReg.SetCarryFlag(carryFlag);
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(byteToShift);
	} else {
		firstArgument.SetData(byteToShift);
	}
}

void CpuInstruction::RotateLeft(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	uint8_t byteToShift = 0;
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		byteToShift = fromDefault.GetData();
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		byteToShift = firstArgument.GetData();
	}
	bool newBit0 = NesDebugger::sStatusReg.GetCarryFlag();
	bool carryFlag = byteToShift & 0b10000000;
	byteToShift = byteToShift << 1;
	bool negFlag = byteToShift & 0b10000000;
	if (newBit0) {
		byteToShift |= 0b00000001;
	}

	NesDebugger::sStatusReg.SetZeroFlag(byteToShift == 0);
	NesDebugger::sStatusReg.SetNegativeFlag(negFlag);
	NesDebugger::sStatusReg.SetCarryFlag(carryFlag);
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(byteToShift);
	} else {
		firstArgument.SetData(byteToShift);
	}
}

void CpuInstruction::ShiftRight(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	uint8_t byteToShift = 0;
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		byteToShift = fromDefault.GetData();
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		byteToShift = firstArgument.GetData();
	}
	bool carryFlag = byteToShift & 0b00000001;
	byteToShift = byteToShift >> 1;
	bool negFlag = ByteHelper::IsNegative(byteToShift);

	NesDebugger::sStatusReg.SetZeroFlag(byteToShift == 0);
	NesDebugger::sStatusReg.SetNegativeFlag(negFlag);
	NesDebugger::sStatusReg.SetCarryFlag(carryFlag);
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(byteToShift);
	} else {
		firstArgument.SetData(byteToShift);
	}
}

void CpuInstruction::ShiftLeft(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(toDefault);

	uint8_t byteToShift = 0;
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		byteToShift = fromDefault.GetData();
	} else {
		if (memoryModRegister != nullptr)
		{
			firstArgument.ModifyMemory(*memoryModRegister);
		}
		byteToShift = firstArgument.GetData();
	}
	bool carryFlag = byteToShift & 0b10000000;
	byteToShift = byteToShift << 1;
	bool negFlag = ByteHelper::IsNegative(byteToShift);

	NesDebugger::sStatusReg.SetZeroFlag(byteToShift == 0);
	NesDebugger::sStatusReg.SetNegativeFlag(negFlag);
	NesDebugger::sStatusReg.SetCarryFlag(carryFlag);
	if (firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed)
	{
		fromDefault.SetData(byteToShift);
	} else {
		firstArgument.SetData(byteToShift);
	}
}

void CpuInstruction::PopByteFromStack(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);

	toDefault.SetData(NesDebugger::PopByteFromStack());
}

void CpuInstruction::PopByteFromStackNegZero(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);

	toDefault.SetData(NesDebugger::PopByteFromStack());
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::PushByteToStack(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(toDefault);

	NesDebugger::PushByteToStack(fromDefault.GetData());
}

void CpuInstruction::JumpFromSubroutine(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	uint16_t toAddress = NesDebugger::PopByteFromStack();
	toAddress |= (NesDebugger::PopByteFromStack() << 8);
	SetActiveInstructionByMemoryAddress(toAddress +1); //We want offset because it'll increment after
}

void CpuInstruction::ReturnFromInterrupt(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	uint16_t toAddress = NesDebugger::PopByteFromStack();
	toAddress |= (NesDebugger::PopByteFromStack() << 8);
	uint8_t statusFlags = NesDebugger::PopByteFromStack();
	NesDebugger::sStatusReg.Set(statusFlags);
	SetActiveInstructionByMemoryAddress(toAddress); //So this one gives the memory address of the jump to EXACTLY. Cool. Thx 6502
}

void CpuInstruction::JumpToSubroutine(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	uint16_t nextOp = NesDebugger::mInstructionList[NesDebugger::mActiveInstruction+1].GetAddress();
	--nextOp; //For some reason, we store the address of the next instruction...minus 1. Ok, I guess
	uint8_t lowByte = nextOp & 0xFF;
	uint8_t highByte = (nextOp >> 8) & 0xFF;
	NesDebugger::PushByteToStack(highByte);
	NesDebugger::PushByteToStack(lowByte);
	SetActiveInstructionByMemoryAddress(firstArgument.GetMemoryLocation());
}

void CpuInstruction::BranchIfOverflowClear(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(!NesDebugger::sStatusReg.GetOverflowFlag(), firstArgument);
}

void CpuInstruction::BranchIfOverflowSet(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(NesDebugger::sStatusReg.GetOverflowFlag(), firstArgument);
}

void CpuInstruction::BranchIfPositive(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(!NesDebugger::sStatusReg.GetNegativeFlag(), firstArgument);
}

void CpuInstruction::BranchIfMinus(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(NesDebugger::sStatusReg.GetNegativeFlag(), firstArgument);
}

void CpuInstruction::BranchIfCarrySet(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(NesDebugger::sStatusReg.GetCarryFlag(), firstArgument);
}

void CpuInstruction::BranchIfCarryClear(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(!NesDebugger::sStatusReg.GetCarryFlag(), firstArgument);
}

void CpuInstruction::BranchIfNotEqualInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(!NesDebugger::sStatusReg.GetZeroFlag(), firstArgument);
}

void CpuInstruction::BranchIfEqualInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	CpuInstruction::ConditionalBranch(NesDebugger::sStatusReg.GetZeroFlag(), firstArgument);
}

void CpuInstruction::ConditionalBranch(bool shouldJump, GenericInstructionTarget& firstArgument)
{
	if (shouldJump)
	{
		uint16_t memoryToJumpTo = NesDebugger::mInstructionList[NesDebugger::mActiveInstruction+1].mAddress;
		int8_t offset = int8_t(firstArgument.GetMemoryLocation()); //unlike most memory types, offset may be negative
		memoryToJumpTo += offset;
		SetActiveInstructionByMemoryAddress(memoryToJumpTo);
	}
}

void CpuInstruction::JumpInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	uint16_t toIndex = firstArgument.GetMemoryLocation();
	SetActiveInstructionByMemoryAddress(toIndex);
}

void CpuInstruction::JumpIndirect(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);

	uint16_t toIndex = firstArgument.GetMemoryLocation();
	uint16_t toAddr = 0;
	if ((toIndex & 0xFF) == 0xFF) //If we're about to go flying off a page boundry...
	{
		toAddr = NesDebugger::mRam.GetMemoryByLocation(toIndex & 0xFF00);
	} else {
		toAddr = NesDebugger::mRam.GetMemoryByLocation(toIndex + 1);
	}
	toAddr = toAddr << 8;
	toAddr = toAddr | NesDebugger::mRam.GetMemoryByLocation(toIndex);
	SetActiveInstructionByMemoryAddress(toAddr);
}

void CpuInstruction::SetActiveInstructionByMemoryAddress(uint16_t address)
{
	auto result = NesDebugger::sOperations.find(address);
	assert(result != NesDebugger::sOperations.end());
	NesDebugger::mActiveInstruction = result->second - 1;
}

void CpuInstruction::SetActiveInstructionByMemoryAddressNoOffset(uint16_t address)
{
	auto result = NesDebugger::sOperations.find(address);
	assert(result != NesDebugger::sOperations.end());
	NesDebugger::mActiveInstruction = result->second;
}

void CpuInstruction::BitTestInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);

	assert(memoryModRegister == nullptr);

	uint8_t andedByte = firstArgument.GetData();
	NesDebugger::sStatusReg.SetNegativeFlag(andedByte & 0b10000000); //This is kinda weird, but it's what the spec says
	NesDebugger::sStatusReg.SetOverflowFlag(andedByte & 0b01000000); //This too, but whatever

	andedByte &= toDefault.GetData();
	NesDebugger::sStatusReg.SetZeroFlag(andedByte == 0);
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
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	assert(firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed);
	assert(memoryModRegister == nullptr);
	toDefault.SetData(fromDefault.GetData());
	NesDebugger::sStatusReg.SetZeroFlag(toDefault.IsZero());
	NesDebugger::sStatusReg.SetNegativeFlag(toDefault.IsNegative());
}

void CpuInstruction::TransferRegisterInstructionNoFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	assert(firstArgument.GetTargetType() == GenericInstructionTarget::TargetType::ENotUsed);
	assert(memoryModRegister == nullptr);
	toDefault.SetData(fromDefault.GetData());
}

void CpuInstruction::StoreDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	if (memoryModRegister != nullptr) {
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	toDefault.SetData(fromDefault.GetData(), firstArgument.GetMemoryLocation(true));
}

void CpuInstruction::LoadDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(fromDefault);
	firstArgument.Reset();
	if (memoryModRegister != nullptr)
	{
		firstArgument.ModifyMemory(*memoryModRegister);
	}
	toDefault.SetData(firstArgument.GetData(), firstArgument.GetMemoryLocation(true));
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

void CpuInstruction::ClearInterruptFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetInterruptDisableFlag(false);
}

void CpuInstruction::ClearOverflowFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetOverflowFlag(false);
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
	NesDebugger::sStatusReg.SetDecimalFlag(true);
}

void CpuInstruction::ClearDecimalFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetDecimalFlag(false);
}

void CpuInstruction::SetCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetCarryFlag(true);
}

void CpuInstruction::ClearCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault)
{
	UNREFERENCED_PARAMETER(firstArgument);
	UNREFERENCED_PARAMETER(memoryModRegister);
	UNREFERENCED_PARAMETER(fromDefault);
	UNREFERENCED_PARAMETER(toDefault);
	NesDebugger::sStatusReg.SetCarryFlag(false);
}

void CpuInstruction::NMIInterrupt()
{
	NesDebugger::PushByteToStack(NesDebugger::sStatusReg.GetRegisterContents());

	uint16_t nextOp = NesDebugger::mInstructionList[NesDebugger::mActiveInstruction].GetAddress();
	uint8_t lowByte = nextOp & 0xFF;
	uint8_t highByte = (nextOp >> 8) & 0xFF;
	NesDebugger::PushByteToStack(highByte);
	NesDebugger::PushByteToStack(lowByte);
	SetActiveInstructionByMemoryAddress(NesDebugger::mNMIVector + 1);
}

CpuInstruction::CpuInstruction()
	:mDescription(CpuInstruction::sOperations.find(0)->second)
{
}

CpuInstruction::CpuInstruction(uint8_t opcode, GenericInstructionTarget firstTarget)
	:mOpCode(opcode), mFirstArgument(firstTarget), mDescription(CpuInstruction::sOperations.find(mOpCode)->second)
{
}

CpuInstruction::CpuInstruction(uint8_t opcode, const OperationDescription& description, uint8_t* argumentLocation, uint16_t address)
	:mOpCode(opcode), mDescription(description), mAddress(address)
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
	case GenericInstructionTarget::EIndirect:
		stringToAppendTo.append("(");
		NesDebugger::PopulateCharBufferWithHex(fullAddress, mFirstArgument.GetLiteralData());
		break;
	case GenericInstructionTarget::ERelative:
		stringToAppendTo.append("*");
		stringToAppendTo.append(std::to_string(int8_t(mFirstArgument.GetMemoryLocation())));
		return;
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

std::string CpuInstruction::GetPrettyHexRep()
{
	std::stringstream stream;
	uint16_t data = mFirstArgument.GetLiteralData();
	uint8_t curByte = uint8_t(data);
	char hexes[5];
	NesDebugger::PopulateCharBufferWithHex(hexes, mOpCode);
	stream << hexes << " ";
	NesDebugger::PopulateCharBufferWithHex(hexes, curByte);
	stream << hexes << " ";
	if (mDescription.mBytes == 3)
	{
		curByte = uint8_t(data >> 8);
		NesDebugger::PopulateCharBufferWithHex(hexes, curByte);
		stream << hexes << " ";
	} else {
		stream << "   ";
	}
	return stream.str();
}
