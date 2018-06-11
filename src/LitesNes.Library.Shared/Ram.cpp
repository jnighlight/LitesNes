#include "pch.h"
#include <ctype.h>
#include "Ram.h"
#include "imgui.h"
#include "NesDebugger.h"

Ram::Ram(std::string name)
	:mName(name), mHiddenName("##")
{
	mHiddenName = "##";
	mHiddenName.append(mName);
	for (uint32_t i = 0; i < 2048; ++i)
	{
		mData[i] += uint8_t(i);
	}
}

Ram::~Ram()
{
}

void Ram::Add(uint8_t addition)
{
	addition;
	//mData += addition;
}

void Ram::Subtract(uint8_t subtraction)
{
	subtraction;
	//mData -= subtraction;
}

void Ram::Render()
{
	ImGui::BeginGroup();
	uint32_t index = 0;
	char text[5];
	for (int line = 0; line < 128; line++)
	{
		ImGui::Text("RAM %04d", line * 10);
		ImGui::SameLine();
		ImGui::Text("\t");
		for (uint32_t i = 0; i < 16; ++i) {
			ImGui::SameLine();
			NesDebugger::PopulateCharBufferWithHex(text, mData[index]);
			++index;
			ImGui::Text(text);
		}
		index -= 16;
		ImGui::SameLine();
		ImGui::Text("\t");
		for (uint32_t i = 0; i < 16; ++i) {
			ImGui::SameLine();
			char asciiChar[2];
			asciiChar[0] = mData[index];
			if (asciiChar[0] == '?' || isspace(uint8_t(asciiChar[0]))) {
				asciiChar[0] = '.';
			}
			asciiChar[1] = '\00';
			ImGui::Text(asciiChar);
			++index;
		}
	}
	ImGui::EndGroup();
}
