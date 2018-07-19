#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>
#include <stack>
#include "Register.h"
#include "StatusRegister.h"
#include "Ram.h"
#include "CpuInstruction.h"

class NesDebugger
{
public:
	struct NesInstruction
	{
		std::string mName = "DefaultInstruction";
		std::string mMemoryLocationBuf = "$0000";
		uint16_t mMemoryLocation = 0;
		bool mSelected = false;
		bool mIsBreakpoint = false;
		NesInstruction(uint16_t memLoc)
			:mMemoryLocation(memLoc)
		{
			UpdateMemoryLocationBuffer();
		}

		void UpdateMemoryLocationBuffer()
		{
			std::stringstream stream;
			stream << "$" << std::setw(4) << std::setfill('0') << std::hex << mMemoryLocation;
			mMemoryLocationBuf = stream.str();
		}
	};

	NesDebugger();
	~NesDebugger();
	void RenderDebugger();
	void RenderMemoryWindow();
	void RenderStackWindow();
	void Update();
	void DrawFromBuffer(uint32_t* texArray);
	static void PopulateCharBufferWithHex(char* buf, uint16_t byteData);
	static void PopulateCharBufferWithHex(char * buf, uint8_t byteData);
	static void PopulateCharBufferWithHex(char * buf, std::byte byte);
	static void PopulateCharBufferWithHexNonTerm(char* buf, uint8_t byteData);
	static void PushByteToStack(uint8_t byte);
	static uint8_t PopByteFromStack();

	static char NibbleToChar(std::byte nybble);
	

	static uint32_t mActiveInstruction;
	static StatusRegister sStatusReg;// = StatusRegister("S");
	static Register mAReg;// = Register("A");
	static Register mXReg;// = Register("X");
	static Register mYReg;// = Register("Y");
	static Register mPCReg;// = Register("PC");
	static Register mSPReg;// = Register("PC");
	static std::vector<uint8_t> mStack;

	static Ram mRam;// = Ram("Ram");
	static std::vector<CpuInstruction> mInstructionList;

	static std::map<uint16_t, uint16_t> sOperations;

private:
	void IncrementActiveInstruction();
	static char NibbleToChar(unsigned char nybble);

	bool mRunning = false;
	bool mShouldStep = false;
	bool mStepped = false;
};
