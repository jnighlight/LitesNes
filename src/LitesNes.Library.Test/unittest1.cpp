#include "pch.h"
#include "CppUnitTest.h"
#include "NesDebugger.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LitesNesLibraryTest
{		
	TEST_CLASS(NesDebuggerUtilityTests)
	{
	public:
		
		TEST_METHOD(TestTestMethod)
		{
			Assert::IsTrue(true);
		}

		TEST_METHOD(uint16tToHexWorks)
		{
			char buffer[5];
			uint16_t alphabetical = 0xabcd;
			NesDebugger::PopulateCharBufferWithHex(buffer, alphabetical);
			Assert::AreEqual(buffer[0], 'A');
			Assert::AreEqual(buffer[1], 'B');
			Assert::AreEqual(buffer[2], 'C');
			Assert::AreEqual(buffer[3], 'D');

			uint16_t unalphabetical = 0xdabc;
			NesDebugger::PopulateCharBufferWithHex(buffer, unalphabetical);
			Assert::AreEqual(buffer[0], 'D');
			Assert::AreEqual(buffer[1], 'A');
			Assert::AreEqual(buffer[2], 'B');
			Assert::AreEqual(buffer[3], 'C');

			uint16_t nums = 0x1234;
			NesDebugger::PopulateCharBufferWithHex(buffer, nums);
			Assert::AreEqual(buffer[0], '1');
			Assert::AreEqual(buffer[1], '2');
			Assert::AreEqual(buffer[2], '3');
			Assert::AreEqual(buffer[3], '4');
		}
	};
}