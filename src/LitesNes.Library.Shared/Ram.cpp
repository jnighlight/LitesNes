#include "Ram.h"
#include "pch.h"
#include <ctype.h>
#include "Ram.h"
#include "imgui.h"
#include "NesDebugger.h"

Ram::Ram()
	:Ram("DefaultName")
{
}

Ram::Ram(std::string name)
	:mName(name), mHiddenName("##")
{
	mHiddenName = "##";
	mHiddenName.append(mName);
	for (uint32_t i = 0; i < 128; ++i)
	{
		std::string curStr;
		curStr.reserve(1024);
		mCachedRamStrings.push_back(curStr);
	}
}

Ram::~Ram()
{
}

void Ram::SetMemoryByLocation(uint16_t memoryLocation, uint8_t data)
{
	/*
	if (memoryLocation > 1024 * 2) {
		throw std::exception("trying to set out of bounds memory location");
	}*/
	mData[memoryLocation] = data;
}

uint8_t Ram::GetMemoryByLocation(uint16_t memoryLocation)
{
	/*
	if (memoryLocation > 0xFFFF * 2) {
		throw std::exception("trying to set out of bounds memory location");
	}*/
	return mData[memoryLocation];
}

void Ram::Render()
{
	ImGui::BeginGroup();
	for (uint32_t i = 0; i < 128; ++i)
	{
		std::string& strref = mCachedRamStrings[i];
		ImGui::Text(strref.c_str());
	}
	ImGui::EndGroup();
	rerenderCount++;
	if (rerenderCount > 120)
	{
		UpdateCachedRamString();
		rerenderCount = 0;
	}
}

void Ram::UpdateCachedRamString()
{
	for (auto& strRef : mCachedRamStrings)
	{
		strRef.clear();
		strRef.reserve(5064);
	}
	char text[(3 * 16) + 2];
	char asciiChar[(2 * 16) + 2];
	uint32_t index = 0;
	for (int line = 0; line < 4095; line++)
	{
		std::string& mCachedRamString = mCachedRamStrings[line / 32];
		mCachedRamString.append("RAM ");
		mCachedRamString.append(int_to_hex(line * 16));
		mCachedRamString.append("\t");
		NesDebugger::PopulateCharBufferWithHexNonTerm(text, mData[index]);
		for (uint32_t i = 0; i < 16; ++i) {
			NesDebugger::PopulateCharBufferWithHexNonTerm((text + i*3), mData[index]);
			++index;
		}
		text[3 * 16] = '\t';
		text[(3 * 16) + 1] = '\00';
		mCachedRamString.append(text);
		index -= 16;
		for (uint32_t i = 0; i < 16; ++i) {
			char* curIndex = (asciiChar + i * 2);
			curIndex[0] = mData[index];
			if (curIndex[0] > 126 || curIndex[0] < 32) {
				curIndex[0] = '.';
			}
			curIndex[1] = ' ';
			++index;
		}
		asciiChar[2 * 16] = '\n';
		asciiChar[(2 * 16) + 1] = '\00';
		mCachedRamString.append(asciiChar);
	}
}

std::string Ram::int_to_hex( int i )
{
  std::stringstream stream;
  stream << "0x" 
         << std::setfill ('0') << std::setw(4) 
         << std::hex << i;
  return stream.str();
}
