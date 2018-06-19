#include "pch.h"
#include "NesDebugger.h"
#include "CppUnitTest.h"
#include "CpuInstruction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LitesNesLibraryTest
{		
	TEST_CLASS(CpuInstructionTests)
	{
	public:
		
		TEST_METHOD(opcodeToCpuInstruction)
		{
			uint8_t testOpCode = 0x94; //STY, Zero Page, XReg

			auto searchResult = CpuInstruction::sOperations.find(testOpCode);
			Assert::IsFalse(searchResult == CpuInstruction::sOperations.end()); //Making sure the argument exists

			NesDebugger::mYReg.Set(0xFE);
			NesDebugger::mXReg.Set(0x02);

			GenericInstructionTarget firstTarget(0x00, GenericInstructionTarget::TargetType::EZeroPage);
			CpuInstruction testInstruction(testOpCode, firstTarget);
			testInstruction.Execute();
			Assert::AreEqual(uint8_t(0xFE), NesDebugger::mRam.GetMemoryByLocation(0x02));
		}
	};
}
