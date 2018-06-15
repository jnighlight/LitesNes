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
	~CpuInstruction();
	void Execute();

	static std::string GetOpcodeName(uint8_t opcode);
	static void TransferDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void StoreDataInstruction(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetInterruptFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetDecimalFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
	static void SetCarryFlag(GenericInstructionTarget& firstArgument, Register* memoryModRegister, GenericInstructionTarget& fromDefault, GenericInstructionTarget& toDefault);
private:
	uint8_t mOpCode;
	GenericInstructionTarget mFirstArgument;
};
