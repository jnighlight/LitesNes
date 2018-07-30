#include "Register.h"
#include "pch.h"
#include "Register.h"
#include "imgui.h"
#include "ByteHelper.h"
#include "NesDebugger.h"

Register::Register()
	:Register("DefaultRegName") {};

Register::Register(std::string name, uint8_t data /*= 0*/)
	:mName(name), mHiddenName("##"), mData(data)
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

void Register::Set(uint8_t inValue)
{
	mData = inValue;
}

void Register::Set(Register & otherReg)
{
	mData = otherReg.mData;
}

bool Register::IsZero() const
{
	return ByteHelper::IsZero(mData);
}

bool Register::IsNegative() const
{
	return ByteHelper::IsNegative(mData);
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
