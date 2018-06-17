#include "Rom.h"
#include "pch.h"
#include <ctype.h>
#include "Rom.h"
#include "imgui.h"
#include "NesDebugger.h"

Rom::Rom()
	:Rom("DefaultName")
{}

Rom::Rom(std::string name)
	:mName(name), mHiddenName("##")
{
	mHiddenName = "##";
	mHiddenName.append(mName);
	for (uint32_t i = 0; i < 2048; ++i)
	{
		mData[i] += uint8_t(i);
	}
}

Rom::~Rom()
{
}