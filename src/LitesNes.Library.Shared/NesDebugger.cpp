#include "pch.h"
#include "NesDebugger.h"
#include "imgui.h"

NesDebugger::NesDebugger()
{
}


NesDebugger::~NesDebugger()
{
}

void NesDebugger::Render()
{
	ImGui::Begin("LitesNesDebugger");
    bool show_demo_window = true;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	static float f = 0.0f;
	static int counter = 0;
	bool steppedThisFrame = false;
	if (ImGui::Button("Run"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	{
		mActiveInstruction++;
		if (mActiveInstruction >= mInstructionList.size())
		{
			mActiveInstruction = 0;
		}
		steppedThisFrame = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Step"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	{
		mActiveInstruction++;
		if (mActiveInstruction >= mInstructionList.size())
		{
			mActiveInstruction = 0;
		}
		steppedThisFrame = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Step Into"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	{
		mActiveInstruction++;
		if (mActiveInstruction >= mInstructionList.size())
		{
			mActiveInstruction = 0;
		}
		steppedThisFrame = true;
	}
	ImGui::Text("A");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##A", mARegBuf, 8, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
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
	ImGui::InputText("##X", mXRegBuf, 8, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("Y");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##Y", mYRegBuf, 8, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();
	ImGui::SameLine();
	ImGui::Text("        ");
	ImGui::SameLine();
	ImGui::Text("PC");
	ImGui::SameLine();
	ImGui::PushItemWidth(40);
	ImGui::InputText("##PC", mPCRegBuf, 8, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase);
	ImGui::PopItemWidth();

	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	/*
	static bool test_read_only = false;
	static char test_text[1024*16] =
		"/*\n"
		" The Pentium F00F bug, shorthand for F0 0F C7 C8,\n"
		" the hexadecimal encoding of one offending instruction,\n"
		" more formally, the invalid operand with locked CMPXCHG8B\n"
		" instruction bug, is a design flaw in the majority of\n"
		" Intel Pentium, Pentium MMX, and Pentium OverDrive\n"
		" processors (all in the P5 microarchitecture).\n"
		"\n\n"
		"label:\n"
		"\tlock cmpxchg8b eax\n";
	static bool titlebool = true;
	static bool scrollbool = false;*/
	ImVec2 size(50, 20);
	/*
	ImGui::BeginGroup();
	{
		ImGui::BeginGroup();
		ImGui::Button("AAA", size);
		ImGui::Button("BBB", size);
		ImGui::BeginGroup();
		ImGui::Button("CCC", size);
		ImGui::Button("DDD", size);
		ImGui::EndGroup();
		//ImGui::SameLine();
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::Button("EEE", size);
		ImGui::EndGroup();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("First group hovered");
	}
	ImGui::SameLine();*/
	ImVec2 totalListSize = size;
	totalListSize.x += totalListSize.x * 7;
	totalListSize.y *= 15;
	size.x *= 4;
	size.y /= 1.5;
	if (ImGui::ListBoxHeader("##Instructions", totalListSize))
	{
		for (uint32_t i = 0; i < mInstructionList.size(); ++i) {
			std::string outString;
			outString.reserve(20);
			if (mInstructionList[i].mIsBreakpoint) {
				outString.append("->");
			} else {
				outString.append("  ");
			}
			outString.append(mInstructionList[i].mMemoryLocation);
			outString.append("\t");
			outString.append(mInstructionList[i].mName);
			if (ImGui::Selectable(outString.c_str(), i == mActiveInstruction, 0, size))
			{
				mInstructionList[i].mIsBreakpoint = !mInstructionList[i].mIsBreakpoint;
			}
			if (steppedThisFrame && i == mActiveInstruction) {
				ImGui::SetScrollHere(0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
			}
		}
	}
	ImGui::ListBoxFooter();
	ImGui::End();
	/*
	ImGui::Checkbox("No titlebar", &titlebool); ImGui::SameLine(150,0);
	ImGui::Checkbox("No scrollbar", &scrollbool); ImGui::SameLine(150,100);
	ImGui::InputTextMultiline("##source", test_text, IM_ARRAYSIZE(test_text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), ImGuiInputTextFlags_AllowTabInput | (test_read_only ? ImGuiInputTextFlags_ReadOnly : 0));
	ImGui::EndGroup();*/
}
