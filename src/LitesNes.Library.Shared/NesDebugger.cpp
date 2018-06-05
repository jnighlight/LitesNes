#include "pch.h"
#include "NesDebugger.h"
#include "imgui.h"

NesDebugger::NesDebugger()
{
}


NesDebugger::~NesDebugger()
{
}

void NesDebugger::Update()
{
	if (mRunning) {
		IncrementActiveInstruction();
		mStepped = true;
		if (mInstructionList[mActiveInstruction].mIsBreakpoint)
		{
			mRunning = false;
		}
		mAReg++;
		PopulateCharBufferWithHex(mARegBuf, mAReg);
		mXReg++;
		PopulateCharBufferWithHex(mXRegBuf, mXReg);
		mYReg++;
		PopulateCharBufferWithHex(mYRegBuf, mYReg);
		mPCReg++;
		PopulateCharBufferWithHex(mPCRegBuf, mPCReg);
	}
}

void NesDebugger::IncrementActiveInstruction()
{
	mActiveInstruction++;
	if (mActiveInstruction >= mInstructionList.size())
	{
		mActiveInstruction = 0;
	}
	mAReg++;
	PopulateCharBufferWithHex(mARegBuf, mAReg);
	mXReg++;
	PopulateCharBufferWithHex(mXRegBuf, mXReg);
	mYReg++;
	PopulateCharBufferWithHex(mYRegBuf, mYReg);
	mPCReg++;
	PopulateCharBufferWithHex(mPCRegBuf, mPCReg);
}

//Assumes char* buf is of at LEAST length 5. 
void NesDebugger::PopulateCharBufferWithHex(char* buf, uint16_t byteData)
{
	uint8_t lowerBytes = ((byteData << 8) >> 8);
	uint8_t higherBytes = (byteData >> 8);
	unsigned char firstNibbleLower = (lowerBytes >> 4);
	unsigned char secondNibbleLower = (lowerBytes & 0x0F);
	unsigned char firstNibbleHigher = (higherBytes >> 4);
	unsigned char secondNibbleHigher = (higherBytes & 0x0F);
	buf[0] = NibbleToChar(firstNibbleHigher);
	buf[1] = NibbleToChar(secondNibbleHigher);
	buf[2] = NibbleToChar(firstNibbleLower);
	buf[3] = NibbleToChar(secondNibbleLower);
	buf[4] = '\00';
}

char NesDebugger::NibbleToChar(unsigned char nybble)
{
	if(nybble<10U)
	{
		 return (char)('0'+nybble);
	}
	else
	{
		 nybble-=10U;
		 return (char)('A'+nybble);
	}
}

void NesDebugger::RenderDebugger()
{
	ImGui::Begin("LitesNesDebugger");
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static float f = 0.0f;
	static int counter = 0;
	if (ImGui::Button("Run"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	{
		mRunning = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Step"))
	{
		IncrementActiveInstruction();
		mStepped = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Step Into"))
	{
		IncrementActiveInstruction();
		mStepped = true;
	}
	ImGui::Text("A");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##A", mARegBuf, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("     ");
	ImGui::SameLine();
	ImGui::Checkbox("N", &show_demo_window);      // Edit bools storing our windows open/close state
	ImGui::SameLine();
	ImGui::Checkbox("V", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("U", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("B", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("D", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("I", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("Z", &show_demo_window);
	ImGui::SameLine();
	ImGui::Checkbox("C", &show_demo_window);


	ImGui::Text("X");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##X", mXRegBuf, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##Y", mYRegBuf, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();
	ImGui::Text("PC");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##PC", mPCRegBuf, 5, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();



	ImVec2 size(50, 20);
	ImVec2 totalListSize = size;
	totalListSize.x += totalListSize.x * 7;
	totalListSize.y *= 15;
	size.x *= 4;
	size.y /= 1.5;
	if (ImGui::ListBoxHeader("##Instructions", totalListSize))
	{
		for (uint16_t i = 0; i < mInstructionList.size(); ++i) {
			std::string outString;
			outString.reserve(20);
			if (mInstructionList[i].mIsBreakpoint) {
				outString.append("->");
			} else {
				outString.append("  ");
			}
			outString.append(mInstructionList[i].mMemoryLocationBuf);
			outString.append("\t");
			outString.append(mInstructionList[i].mName);
			if (ImGui::Selectable(outString.c_str(), i == mActiveInstruction, 0, size))
			{
				mInstructionList[i].mIsBreakpoint = !mInstructionList[i].mIsBreakpoint;
			}
			if (mStepped && i == mActiveInstruction) {
				ImGui::SetScrollHere(0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
			}
		}
	}
	ImGui::ListBoxFooter();
	ImGui::End();
}

void NesDebugger::RenderMemoryWindow()
{
	ImGui::Begin("LitesNesMemory");

	ImGui::BeginGroup();
	for (int line = 0; line < 100; line++)
	{
		ImGui::Text("RAM %04d", line * 10);
		ImGui::SameLine();
		ImGui::Text("\t");
		for (uint32_t i = 0; i < 16; ++i) {
			ImGui::SameLine();
			ImGui::Text("00");
		}
		ImGui::SameLine();
		ImGui::Text("\t");
		for (uint32_t i = 0; i < 16; ++i) {
			ImGui::SameLine();
			ImGui::Text(".");
		}
	}
	ImGui::EndGroup();

	ImGui::End();
}
