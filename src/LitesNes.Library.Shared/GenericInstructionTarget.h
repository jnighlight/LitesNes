#pragma once
#include <stdint.h>
#include "Ram.h"
#include "Register.h"

class GenericInstructionTarget
{
public:
	enum TargetType {
		EInvalid = 0,
		ENotUsed,
		ERegister,
		ERam,
		EAbsolute,
		EZeroPage,
		EImmediate,
		EIndexedIndirect,
		EIndirectIndexed,
		EMax
	};

	union DataSource {
		Ram* mRam;
		Register* mRegister;
	};

	GenericInstructionTarget();
	GenericInstructionTarget(Ram& ram);
	GenericInstructionTarget(Register& inReg);
	GenericInstructionTarget(uint8_t inByte, TargetType type);
	~GenericInstructionTarget();

	void SetTargetType(TargetType targetType) { mTargetType = targetType; };
	TargetType GetTargetType() { return mTargetType; };

	void SetDataSource(Ram& ram, TargetType targetType) {
		Reset(); mDataSource.mRam = &ram; SetTargetType(targetType);
	};
	void SetDataSource(Register& inReg) {
		Reset(); mDataSource.mRegister = &inReg; SetTargetType(ERegister);
	}
	void SetLiteralData(uint16_t literalData) { mLiteralByte = literalData; };
	uint16_t GetLiteralData() { return mLiteralByte; };

	void SetData(uint8_t data);
	void SetData(uint8_t data, uint16_t location);
	uint8_t GetData();
	void Reset();

	bool IsZero();
	bool IsNegative();
	void ModifyMemory(Register& registerToModifyBy);
	uint16_t GetMemoryLocation() {
		if (!mHasBeenModified) {
			mModifiedByte = mLiteralByte;
			mHasBeenModified = true;
		}
		return mModifiedByte;
	};

private:
	TargetType mTargetType = EInvalid;
	uint16_t mLiteralByte = 0;
	uint16_t mModifiedByte = 0;
	bool mHasBeenModified = false;
	DataSource mDataSource;
};

