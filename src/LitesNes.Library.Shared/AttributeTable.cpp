#include "pch.h"
#include "AttributeTable.h"
#include <ctype.h>
#include "AttributeTable.h"
#include "imgui.h"
#include "NesDebugger.h"

AttributeTable::AttributeTable()
	:AttributeTable("DefaultName")
{}

AttributeTable::AttributeTable(std::string name)
	:mName(name), mHiddenName("##")
{
	mHiddenName = "##";
	mHiddenName.append(mName);
	/*
	for (uint32_t i = 0; i < 2048; ++i)
	{
		mData[i] += uint8_t(i);
	}*/
}

AttributeTable::~AttributeTable()
{
}

void AttributeTable::Add(uint8_t addition)
{
	addition;
	//mData += addition;
}

void AttributeTable::Subtract(uint8_t subtraction)
{
	subtraction;
	//mData -= subtraction;
}

void AttributeTable::SetMemoryByLocation(uint16_t memoryLocation, uint8_t data)
{
	if (memoryLocation > 1024 * 2) {
		throw std::exception("trying to set out of bounds memory location");
	}
	mData[memoryLocation] = data;
}

uint8_t AttributeTable::GetMemoryByLocation(uint16_t memoryLocation)
{
	if (memoryLocation > 1024 * 2) {
		throw std::exception("trying to set out of bounds memory location");
	}
	return mData[memoryLocation];
}

void AttributeTable::Render()
{
	ImGui::BeginGroup();
	uint32_t index = 0;
	char text[5];
	for (int line = 0; line < 128; line++)
	{
		ImGui::Text("RAM %04X", line * 16);
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
			if (asciiChar[0] > 126 || asciiChar[0] < 32) {
				asciiChar[0] = '.';
			}
			asciiChar[1] = '\00';
			ImGui::Text(asciiChar);
			++index;
		}
	}
	ImGui::EndGroup();
}
