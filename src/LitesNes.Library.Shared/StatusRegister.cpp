#include "StatusRegister.h"
#include "StatusRegister.h"
#include "StatusRegister.h"
#include "StatusRegister.h"
#include "pch.h"
#include "StatusRegister.h"
#include "imgui.h"
#include "NesDebugger.h"

StatusRegister::StatusRegister(std::string name)
	:Register(name)
{
}

StatusRegister::~StatusRegister()
{
}

void StatusRegister::Render()
{
	std::byte statusByte = std::byte(mData);
	bool curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::Text("     ");
	ImGui::SameLine();
	ImGui::Checkbox("N", &curFlagIsSet);      // Edit bools storing our windows open/close state
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("V", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("U", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("B", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("D", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("I", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("Z", &curFlagIsSet);
	curFlagIsSet = uint8_t(statusByte & std::byte(1U));
	statusByte = statusByte >> 1U;
	ImGui::SameLine();
	ImGui::Checkbox("C", &curFlagIsSet);
}

void StatusRegister::SetZeroFlag(bool zeroFlag)
{
	std::byte* statusByte = reinterpret_cast<std::byte*>(&mData);
	if (zeroFlag) {
		(*statusByte) |= std::byte(0b01000000);
	} else {
		(*statusByte) &= std::byte(0b10111111);
	}
}

bool StatusRegister::GetZeroFlag()
{
	return mData & 0b01000000;
}

void StatusRegister::SetNegativeFlag(bool negativeFlag)
{
	std::byte* statusByte = reinterpret_cast<std::byte*>(&mData);
	if (negativeFlag) {
		(*statusByte) |= std::byte(0b00000001);
	} else {
		(*statusByte) &= std::byte(0b11111110);
	}
}

bool StatusRegister::GetNegativeFlag()
{
	return mData & 0b00000001;
}

void StatusRegister::SetInterruptDisableFlag(bool interruptDisableFlag)
{
	std::byte* statusByte = reinterpret_cast<std::byte*>(&mData);
	if (interruptDisableFlag) {
		(*statusByte) |= std::byte(0b00100000);
	} else {
		(*statusByte) &= std::byte(0b11011111);
	}
}

bool StatusRegister::GetInterruptDisableFlag()
{
	return mData & 0b00100000;
}

void StatusRegister::SetCarryFlag(bool carryFlag)
{
	std::byte* statusByte = reinterpret_cast<std::byte*>(&mData);
	if (carryFlag) {
		(*statusByte) |= std::byte(0b10000000);
	} else {
		(*statusByte) &= std::byte(0b01111111);
	}
}

bool StatusRegister::GetCarryFlag()
{
	return mData & 0b10000000;
}

void StatusRegister::SetOverflowFlag(bool overflowFlag)
{
	std::byte* statusByte = reinterpret_cast<std::byte*>(&mData);
	if (overflowFlag) {
		(*statusByte) |= std::byte(0b00000010);
	} else {
		(*statusByte) &= std::byte(0b11111101);
	}
}

bool StatusRegister::GetOverflowFlag()
{
	return mData & 0b00000010;
}
