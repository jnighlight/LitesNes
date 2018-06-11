#include "pch.h"
#include "NesDebugger.h"
#include "CppUnitTest.h"
#include "CpuInstruction.h"
#include "CpuInstructionRegisterTransfer.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LitesNesLibraryTest
{		
	TEST_CLASS(CpuInstructionTests)
	{
	public:
		
		TEST_METHOD(RegisterTransferCorrectlyUpdatesStateFlags)
		{
			Register aRegister("A");
			Register bRegister("B");
			uint8_t negByte = 0b10000000;
			uint8_t nonNegByte = 0b01111111;
			CpuInstructionRegisterTransfer transferInstruction(aRegister, bRegister);

			aRegister.Set(nonNegByte);
			transferInstruction.Execute();
			Assert::IsFalse(NesDebugger::sStatusReg.GetNegativeFlag());
			Assert::IsFalse(NesDebugger::sStatusReg.GetZeroFlag());
			aRegister.Set(negByte);
			transferInstruction.Execute();
			Assert::IsTrue(NesDebugger::sStatusReg.GetNegativeFlag());
			Assert::IsFalse(NesDebugger::sStatusReg.GetZeroFlag());
		}
	};
}
