#pragma once
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <windows.h>
#include "GenericInstructionTarget.h"

class CpuInstruction
{
public:
	struct OperationDescription
	{
		OperationDescription()
		{};

		OperationDescription(std::string opcodeName,
							uint8_t byteCount,
							uint8_t cycleCount,
							GenericInstructionTarget::TargetType firstArgType,
							Register* memoryModRegister,
							Ram& defaultFromRam,
							Register& defaultToRegister,
							std::function<void(GenericInstructionTarget&, Register*, GenericInstructionTarget&, GenericInstructionTarget&)> function)
		:mOpcodeName(opcodeName)
		,mBytes(byteCount)
		,mCycles(cycleCount)
		,mFirstArgumentType(firstArgType)
		,mMemoryModReg(memoryModRegister)
		,mDefaultFromTarget(defaultFromRam)
		,mDefaultToTarget(defaultToRegister)
		,mExecuteFunction(function)
		{};

		OperationDescription(std::string opcodeName,
							uint8_t byteCount,
							uint8_t cycleCount,
							GenericInstructionTarget::TargetType firstArgType,
							Register* memoryModRegister,
							Register& defaultFromRegister,
							Ram& defaultToRam,
							std::function<void(GenericInstructionTarget&, Register*, GenericInstructionTarget&, GenericInstructionTarget&)> function)
		:mOpcodeName(opcodeName)
		,mBytes(byteCount)
		,mCycles(cycleCount)
		,mFirstArgumentType(firstArgType)
		,mMemoryModReg(memoryModRegister)
		,mDefaultFromTarget(defaultFromRegister)
		,mDefaultToTarget(defaultToRam)
		,mExecuteFunction(function)
		{};

		OperationDescription(std::string opcodeName,
							uint8_t byteCount,
							uint8_t cycleCount,
							GenericInstructionTarget::TargetType firstArgType,
							Register* memoryModRegister,
							Register& defaultFromRegister,
							Register& defaultToRegister,
							std::function<void(GenericInstructionTarget&, Register*, GenericInstructionTarget&, GenericInstructionTarget&)> function)
		:mOpcodeName(opcodeName)
		,mBytes(byteCount)
		,mCycles(cycleCount)
		,mFirstArgumentType(firstArgType)
		,mMemoryModReg(memoryModRegister)
		,mDefaultFromTarget(defaultFromRegister)
		,mDefaultToTarget(defaultToRegister)
		,mExecuteFunction(function)
		{};

		std::string mOpcodeName = "XXX";
		uint8_t mBytes = 0;
		uint8_t mCycles = 0;
		GenericInstructionTarget::TargetType mFirstArgumentType = GenericInstructionTarget::TargetType::EInvalid;
		Register* mMemoryModReg = nullptr;
		GenericInstructionTarget mDefaultFromTarget;
		GenericInstructionTarget mDefaultToTarget;
		std::function<void(GenericInstructionTarget&, Register*, GenericInstructionTarget&, GenericInstructionTarget&)> mExecuteFunction;
	};

	static std::map<uint8_t, OperationDescription> sOperations;

	CpuInstruction();
	CpuInstruction(uint8_t opcode, GenericInstructionTarget memoryLocation);
	CpuInstruction(uint8_t opcode, const OperationDescription& description, uint8_t* argumentLocation, uint16_t address);
	~CpuInstruction();
	void Execute();

	static std::string GetOpcodeName(uint8_t opcode);
	static void StoreDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void LoadDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetInterruptFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ClearInterruptFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetDecimalFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ClearDecimalFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ClearCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ClearOverflowFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void NoOperation(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void TransferRegisterInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void TransferRegisterInstructionNoFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void AndOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void EorOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void OrOperationInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BitTestInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void JumpInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void JumpIndirect(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfEqualInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfNotEqualInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfCarrySet(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfCarryClear(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfPositive(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfMinus(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfOverflowSet(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void BranchIfOverflowClear(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void JumpFromSubroutine(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void JumpToSubroutine(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ReturnFromInterrupt(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void PushByteToStack(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void PopByteFromStack(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void PopByteFromStackNegZero(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ShiftLeft(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void ShiftRight(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void RotateLeft(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void RotateRight(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void Increment(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void Decrement(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void AddWithCarryInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SubtractWithCarryInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void CompareWithRegister(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void NMIInterrupt();

	static void SetActiveInstructionByMemoryAddress(uint16_t address);
	static void SetActiveInstructionByMemoryAddressNoOffset(uint16_t address);
	static void ConditionalBranch(bool shouldJump, GenericInstructionTarget& firstArgument);
	static void AddWithCarryLogic(uint8_t acc, uint8_t arg, GenericInstructionTarget& spotToStore);

	bool GetIsBreakpoint() { return mIsBreakPoint; };
	std::string GetOperationName() { return mDescription.mOpcodeName; };
	void GetArgumentDescription(std::string& stringToAppendTo);
	void GetRegOffsetString(std::string& stringToAppendTo);
	void ToggleIsBreakpoint() { mIsBreakPoint = !mIsBreakPoint; };
	uint16_t GetAddress() { return mAddress; };
	uint8_t GetOpCode() { return mOpCode; };
	std::string GetPrettyHexRep();
private:
	uint8_t mOpCode;
	uint16_t mAddress;
	GenericInstructionTarget mFirstArgument;
	const OperationDescription& mDescription;

	bool mIsBreakPoint = false;
};
