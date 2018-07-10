#include "pch.h"
#include "GenericInstructionTarget.h"
#include "ByteHelper.h"
#include "NesDebugger.h"
#include "PPU.h"

GenericInstructionTarget::GenericInstructionTarget()
{
}

GenericInstructionTarget::GenericInstructionTarget(uint8_t inByte, TargetType type)
	:mLiteralByte(inByte), mTargetType(type)
{
}

GenericInstructionTarget::GenericInstructionTarget(Ram& ram)
{
	mDataSource.mRam = &ram;
	mTargetType = ERam;
}

GenericInstructionTarget::GenericInstructionTarget(Register& inReg)
{
	mDataSource.mRegister = &inReg;
	mTargetType = ERegister;
}

bool GenericInstructionTarget::IsNegative()
{
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Checking a not used arg if it's 0"));
		break;
	case GenericInstructionTarget::ERegister:
		return mDataSource.mRegister->IsNegative();
		break;
	case GenericInstructionTarget::EAbsolute:
	case GenericInstructionTarget::EZeroPage:
	case GenericInstructionTarget::EImmediate:
	case GenericInstructionTarget::EIndexedIndirect:
	case GenericInstructionTarget::EIndirectIndexed:
		if (!mHasBeenModified) {
			CheckForSpecialAddress();
			mModifiedByte = mLiteralByte;
			mHasBeenModified = true;
		}
		return ByteHelper::IsNegative(mDataSource.mRam->GetMemoryByLocation(mModifiedByte));
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	return false;
}

bool GenericInstructionTarget::IsZero()
{
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Checking a not used arg if it's 0"));
		break;
	case GenericInstructionTarget::ERegister:
		return mDataSource.mRegister->IsZero();
		break;
	case GenericInstructionTarget::EAbsolute:
	case GenericInstructionTarget::EZeroPage:
	case GenericInstructionTarget::EImmediate:
	case GenericInstructionTarget::EIndexedIndirect:
	case GenericInstructionTarget::EIndirectIndexed:
		if (!mHasBeenModified) {
			CheckForSpecialAddress();
			mModifiedByte = mLiteralByte;
			mHasBeenModified = true;
		}
		return ByteHelper::IsZero(mDataSource.mRam->GetMemoryByLocation(mModifiedByte));
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	return false;
}

uint8_t GenericInstructionTarget::GetData()
{
	CheckForSpecialAddress();
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Checking a not used arg if it's 0"));
		break;
	case GenericInstructionTarget::ERegister:
		return mDataSource.mRegister->GetRegisterContents();
		break;
	case GenericInstructionTarget::EImmediate:
		return uint8_t(mLiteralByte & 0xFF); //Immediates are always 1 byte
		break;
	case GenericInstructionTarget::ERam:
	case GenericInstructionTarget::EAbsolute:
	case GenericInstructionTarget::EZeroPage:
	case GenericInstructionTarget::EIndexedIndirect:
	case GenericInstructionTarget::EIndirectIndexed:
		if (!mHasBeenModified) {
			CheckForSpecialAddress();
			mModifiedByte = mLiteralByte;
			mHasBeenModified = true;
		}
		return mDataSource.mRam->GetMemoryByLocation(mModifiedByte);
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	throw(std::exception("Wasn't any type DIE DIE DIE"));
}

void GenericInstructionTarget::ModifyMemory(Register& registerToModifyBy)
{
	uint16_t actualAddress = 0;
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Modifying a not used memory location. Weird."));
		break;
	case GenericInstructionTarget::ERegister:
		throw(std::exception("Register modifying register, something's up"));
		break;
	case GenericInstructionTarget::EAbsolute:
		mModifiedByte = mLiteralByte + registerToModifyBy.GetRegisterContents();
		break;
	case GenericInstructionTarget::EZeroPage:
		mModifiedByte = (mLiteralByte + registerToModifyBy.GetRegisterContents()) & 0xFF;
		break;
	case GenericInstructionTarget::EImmediate:
		throw(std::exception("Register Modifying Immediate, that's not rite..."));
		break;
	case GenericInstructionTarget::EIndirect:
		mModifiedByte = mDataSource.mRam->GetMemoryByLocation(mLiteralByte);
		mModifiedByte = mModifiedByte << 8;
		mModifiedByte &= mDataSource.mRam->GetMemoryByLocation(mLiteralByte+1);
		break;
	case GenericInstructionTarget::EIndexedIndirect:
		mModifiedByte = mLiteralByte + NesDebugger::mXReg.GetRegisterContents();
		mModifiedByte &= 0xFF;
		actualAddress = mDataSource.mRam->GetMemoryByLocation(mModifiedByte+1);
		actualAddress <<= 8;
		actualAddress |= mDataSource.mRam->GetMemoryByLocation(mModifiedByte);
		mModifiedByte = actualAddress;
		break;
	case GenericInstructionTarget::EIndirectIndexed:
		mModifiedByte = mLiteralByte;
		mModifiedByte &= 0xFF;
		actualAddress = mDataSource.mRam->GetMemoryByLocation(mModifiedByte+1);
		actualAddress <<= 8;
		actualAddress |= mDataSource.mRam->GetMemoryByLocation(mModifiedByte);
		actualAddress += NesDebugger::mYReg.GetRegisterContents();
		mModifiedByte = actualAddress;
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
	mHasBeenModified = true;
	CheckForSpecialAddress();
}

void GenericInstructionTarget::CheckForSpecialAddress()
{
	if (mModifiedByte >= 0x0800 && mModifiedByte <= 0x1FFF) {
		mModifiedByte = mModifiedByte % 0x0800;
	}
	if ((mModifiedByte >= 0x2000 && mModifiedByte <= 0x2007) || mModifiedByte == 0x4014) {
		switch (mModifiedByte)
		{
		case 0x2000:
			mDataSource.mRegister = &PPU::PPUCTRL;
			break;
		case 0x2001:
			mDataSource.mRegister = &PPU::PPUMASK;
			break;
		case 0x2002:
			mDataSource.mRegister = &PPU::PPUSTATUS;
			break;
		case 0x2003:
			mDataSource.mRegister = &PPU::OAMADDR;
			break;
		case 0x2004:
			mDataSource.mRegister = &PPU::OAMDATA;
			break;
		case 0x2005:
			mDataSource.mRegister = &PPU::PPUSCROLL;
			break;
		case 0x2006:
			mDataSource.mRegister = &PPU::PPUADDR;
			break;
		case 0x2007:
			mDataSource.mRegister = &PPU::PPUDATA;
			break;
		case 0x4014:
			mDataSource.mRegister = &PPU::OAMDMA;
			break;
		default:
			break;
		}
		mTargetType = ERegister;
	}
	if (mModifiedByte >= 0x2008 && mModifiedByte <= 0x3FFF) {
		mModifiedByte = 0x2000 + (mModifiedByte % 0x0008);
	}
}

void GenericInstructionTarget::SetData(uint8_t data)
{
	uint16_t actualAddress = 0;
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Modifying a not used memory location. Weird."));
		break;
	case GenericInstructionTarget::ERegister:
		mDataSource.mRegister->Set(data);
		break;
	case GenericInstructionTarget::ERam:
		mDataSource.mRam->SetMemoryByLocation(mModifiedByte, data);
		break;
	case GenericInstructionTarget::EAbsolute:
		mDataSource.mRam->SetMemoryByLocation(mModifiedByte, data);
		break;
	case GenericInstructionTarget::EZeroPage:
		mDataSource.mRam->SetMemoryByLocation(uint16_t(mModifiedByte & 0xFF), data);
		break;
	case GenericInstructionTarget::EImmediate:
		throw(std::exception("Register Modifying Immediate, that's not rite..."));
		break;
	case GenericInstructionTarget::EIndexedIndirect:
		mModifiedByte = mLiteralByte + NesDebugger::mXReg.GetRegisterContents();
		mModifiedByte &= 0xFF;
		actualAddress = mDataSource.mRam->GetMemoryByLocation(mModifiedByte+1);
		actualAddress <<= 8;
		actualAddress |= mDataSource.mRam->GetMemoryByLocation(mModifiedByte);
		mDataSource.mRam->SetMemoryByLocation(actualAddress, data);
		//TODO: Get zero page val, add X to it, then point to the resultant memory
		break;
	case GenericInstructionTarget::EIndirectIndexed:
		//TODO: Get zero page val, store data at location, add Y to it, then point to the resultant memory
		mModifiedByte = mLiteralByte;
		mModifiedByte &= 0xFF;
		actualAddress = mDataSource.mRam->GetMemoryByLocation(mModifiedByte+1);
		actualAddress <<= 8;
		actualAddress |= mDataSource.mRam->GetMemoryByLocation(mModifiedByte);
		actualAddress += NesDebugger::mYReg.GetRegisterContents();
		mDataSource.mRam->SetMemoryByLocation(actualAddress, data);
		break;
	case GenericInstructionTarget::EMax:
		break;
	default:
		throw(std::exception("Wat the fck?"));
	}
}

void GenericInstructionTarget::SetData(uint8_t data, uint16_t location)
{
	switch (mTargetType)
	{
	case GenericInstructionTarget::EInvalid:
		throw(std::exception("Invalid GenericInstructionType"));
		break;
	case GenericInstructionTarget::ENotUsed:
		throw(std::exception("Modifying a not used memory location. Weird."));
		break;
	case GenericInstructionTarget::ERegister:
		mDataSource.mRegister->Set(data);
		break;
	case GenericInstructionTarget::ERam:
		mDataSource.mRam->SetMemoryByLocation(location, data);
		break;
	case GenericInstructionTarget::EAbsolute:
		mDataSource.mRam->SetMemoryByLocation(location, data);
		break;
	case GenericInstructionTarget::EZeroPage:
		mDataSource.mRam->SetMemoryByLocation(location, data);
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
}

void GenericInstructionTarget::Reset()
{
	mModifiedByte = 0;
	mHasBeenModified = false;
}

GenericInstructionTarget::~GenericInstructionTarget()
{
}
