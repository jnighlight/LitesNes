#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>
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
	void Update();
	static void PopulateCharBufferWithHex(char* buf, uint16_t byteData);
	static void PopulateCharBufferWithHex(char * buf, uint8_t byteData);
	static void PopulateCharBufferWithHex(char * buf, std::byte byte);

	static char NibbleToChar(std::byte nybble);
	
	std::vector<CpuInstruction> mInstructionList;
	uint32_t mActiveInstruction = 0;

	static StatusRegister sStatusReg;// = StatusRegister("S");
	static Register mAReg;// = Register("A");
	static Register mXReg;// = Register("X");
	static Register mYReg;// = Register("Y");
	static Register mPCReg;// = Register("PC");
	static Register mSPReg;// = Register("PC");

	static Ram mRam;// = Ram("Ram");
private:
	void IncrementActiveInstruction();
	static char NibbleToChar(unsigned char nybble);


	bool mRunning = false;
	bool mShouldStep = false;
	bool mStepped = false;
};
