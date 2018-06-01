#include "pch.h"
#include "CppUnitTest.h"
#include "IncludeClass.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace LitesNesLibraryTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestTestMethod)
		{
			Assert::IsTrue(true);
		}
	};
}