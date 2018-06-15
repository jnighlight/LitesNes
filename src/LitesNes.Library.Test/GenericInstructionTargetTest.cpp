#include "pch.h"
#include "CppUnitTest.h"
#include "GenericInstructionTarget.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<uint16_t>(const uint16_t& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace LitesNesLibraryTest
{		
	TEST_CLASS(GenericInstructionTargetTest)
	{
	public:
		uint8_t mDefaultRegValue = 5;
		GenericInstructionTarget mGenericInstructionTarget;
		Ram mRam;
		Register mRegister;
		uint8_t mGoalValue = 255;

		TEST_METHOD_INITIALIZE(setup)
		{
			for (uint16_t i = 0; i < 2048; ++i)
			{
				mRam.SetMemoryByLocation(i, uint8_t(i));
			}
			mRegister.Set(mDefaultRegValue);
		}

		TEST_METHOD(ModifyingAbsoluteValuesWorks)
		{
			uint8_t startingMemLoc = 5;
			uint8_t memoryToGrab = startingMemLoc + mDefaultRegValue;
			mRam.SetMemoryByLocation(memoryToGrab, mGoalValue);

			mGenericInstructionTarget.SetDataSource(mRam, GenericInstructionTarget::EAbsolute);
			mGenericInstructionTarget.SetLiteralData(startingMemLoc);
			mGenericInstructionTarget.ModifyMemory(mRegister);
			Assert::AreEqual(uint8_t(memoryToGrab), uint8_t(mGenericInstructionTarget.GetMemoryLocation()));
			Assert::AreEqual(mGoalValue, mGenericInstructionTarget.GetData());
		}

		TEST_METHOD(ModifyingAbsoluteValuesWorksFor16BitMemory)
		{
			uint8_t startingMemLoc = 254;
			uint16_t memoryToGrab = startingMemLoc + mDefaultRegValue;
			mRam.SetMemoryByLocation(memoryToGrab, mGoalValue);

			mGenericInstructionTarget.SetDataSource(mRam, GenericInstructionTarget::EAbsolute);
			mGenericInstructionTarget.SetLiteralData(startingMemLoc);
			mGenericInstructionTarget.ModifyMemory(mRegister);
			Assert::AreEqual(memoryToGrab, mGenericInstructionTarget.GetMemoryLocation());
			Assert::IsTrue(mGenericInstructionTarget.GetMemoryLocation() > 255);
			Assert::AreEqual(mGoalValue, mGenericInstructionTarget.GetData());
		}

		TEST_METHOD(ModifyingZeroPageValuesWorks)
		{
			uint8_t startingMemLoc = 254;
			uint16_t memoryToGrab = startingMemLoc + mDefaultRegValue;
			Assert::IsTrue(memoryToGrab > 255);
			//We want EZeroPage values to wrap around
			memoryToGrab &= 0xFF;
			mRam.SetMemoryByLocation(memoryToGrab, mGoalValue);

			mGenericInstructionTarget.SetDataSource(mRam, GenericInstructionTarget::EZeroPage);
			mGenericInstructionTarget.SetLiteralData(startingMemLoc);
			mGenericInstructionTarget.ModifyMemory(mRegister);
			Assert::AreEqual(uint8_t(memoryToGrab), uint8_t(mGenericInstructionTarget.GetMemoryLocation()));
			Assert::IsFalse(mGenericInstructionTarget.GetMemoryLocation() > 255);
			Assert::AreEqual(mGoalValue, mGenericInstructionTarget.GetData());
		}
	};
}
