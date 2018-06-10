#include "pch.h"
#include "Register.h"
#include "imgui.h"
#include "NesDebugger.h"

Register::Register(std::string name)
	:mName(name), mHiddenName("##")
{
	mHiddenName = "##";
	mHiddenName.append(mName);
}

Register::~Register()
{
}

void Register::Add(uint8_t addition)
{
	mData += addition;
}

void Register::Subtract(uint8_t subtraction)
{
	mData -= subtraction;
}

void Register::Render()
{

	ImGui::Text(mName.c_str());
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	char text[5];
	NesDebugger::PopulateCharBufferWithHex(text, mData);
	ImGui::InputText(mHiddenName.c_str(), text, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
}