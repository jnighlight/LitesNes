#include "pch.h"
#include "NesDebugger.h"
#include "imgui.h"

StatusRegister NesDebugger::sStatusReg = StatusRegister("S");
Register NesDebugger::mAReg = Register("A");
Register NesDebugger::mXReg = Register("X");
Register NesDebugger::mYReg = Register("Y");
Register NesDebugger::mPCReg = Register("PC");
Register NesDebugger::mSPReg = Register("SP");

Ram NesDebugger::mRam = Ram("Ram");

NesDebugger::NesDebugger()
{
	mSPReg.Set(0xFD);
}

NesDebugger::~NesDebugger()
{
}

void NesDebugger::Update()
{
	if (mRunning) {
		IncrementActiveInstruction();
		mStepped = true;
		if (mInstructionList[mActiveInstruction].GetIsBreakpoint())
		{
			mRunning = false;
		}
	}
}

void NesDebugger::IncrementActiveInstruction()
{
	mInstructionList[mActiveInstruction].Execute();
	mActiveInstruction++;
	if (mActiveInstruction >= mInstructionList.size())
	{
		mActiveInstruction = 0;
	}
	mPCReg.Add(1);
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

void NesDebugger::PopulateCharBufferWithHex(char* buf, uint8_t byteData)
{
	unsigned char firstNibbleLower = (byteData >> 4);
	unsigned char secondNibbleLower = (byteData & 0x0F);
	buf[0] = NibbleToChar(firstNibbleLower);
	buf[1] = NibbleToChar(secondNibbleLower);
	buf[2] = '\00';
}

//Assumes char* buf is of at LEAST length 3. 
void NesDebugger::PopulateCharBufferWithHex(char* buf, std::byte inByte)
{
	std::byte firstNibbleLower = (inByte >> 4);
	std::byte secondNibbleLower = (inByte & static_cast<std::byte>(0x0F));
	buf[2] = NibbleToChar(firstNibbleLower);
	buf[3] = NibbleToChar(secondNibbleLower);
	buf[4] = '\00';
}

char NesDebugger::NibbleToChar(std::byte nybble)
{
	uint8_t arithmeticByte = std::to_integer<uint8_t>(nybble);
	if(arithmeticByte < 0xA)
	{
		 return (char)('0' + arithmeticByte);
	}
	else
	{
		 arithmeticByte -= 10U;
		 return (char)('A' + arithmeticByte);
	}
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
	mAReg.Render();
	ImGui::SameLine();
	sStatusReg.Render();


	mXReg.Render();
	ImGui::SameLine();
	mYReg.Render();
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();
	mPCReg.Render();


	ImVec2 size(50, 20);
	ImVec2 totalListSize = size;
	totalListSize.x += totalListSize.x * 7;
	totalListSize.y *= 15;
	size.x *= 4;
	size.y /= 1.5;
	if (ImGui::ListBoxHeader("##Instructions", totalListSize))
	{
		std::string outString;
		outString.reserve(30);
		for (uint16_t i = 0; i < mInstructionList.size(); ++i) {
			outString.clear();
			if (mInstructionList[i].GetIsBreakpoint()) {
				outString.append("->");
			} else {
				outString.append("  ");
			}
			//outString.append(mInstructionList[i].mMemoryLocationBuf);
			outString.append("00\t");
			outString.append(mInstructionList[i].GetOperationName());
			outString.append("\t");
			mInstructionList[i].GetArgumentDescription(outString);
			if (ImGui::Selectable(outString.c_str(), i == mActiveInstruction, 0, size))
			{
				mInstructionList[i].ToggleIsBreakpoint();
			}
			if (mStepped && i == mActiveInstruction) {
				ImGui::SetScrollHere(0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
				mStepped = false;
			}
		}
	}
	ImGui::ListBoxFooter();
	ImGui::End();
}

void NesDebugger::RenderMemoryWindow()
{
	ImGui::Begin("LitesNesMemory");
	mRam.Render();
	ImGui::End();
}
