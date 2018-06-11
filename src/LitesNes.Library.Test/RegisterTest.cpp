#include "pch.h"
#include "CppUnitTest.h"
#include "Register.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LitesNesLibraryTest
{		
	TEST_CLASS(RegisterTests)
	{
	public:
		
		TEST_METHOD(IsNegativeWorks)
		{
			Register TestRegister("test");
			uint8_t negByte = 0b10000000;
			uint8_t nonNegByte = 0b01111111;
			TestRegister.Set(nonNegByte);
			Assert::IsFalse(TestRegister.IsNegative());
			TestRegister.Set(negByte);
			Assert::IsTrue(TestRegister.IsNegative());
		}
	};
}
