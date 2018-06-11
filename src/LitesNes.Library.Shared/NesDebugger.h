#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>
#include "Register.h"
#include "StatusRegister.h"
#include "Ram.h"

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
	
	std::vector<NesInstruction> mInstructionList;
	uint32_t mActiveInstruction = 0;
	static StatusRegister sStatusReg;// = StatusRegister("S");
private:
	void IncrementActiveInstruction();
	static char NibbleToChar(unsigned char nybble);

	Register mAReg = Register("A");
	Register mXReg = Register("X");
	Register mYReg = Register("Y");
	Register mPCReg = Register("PC");

	Ram mRam = Ram("Ram");

	bool mRunning = false;
	bool mShouldStep = false;
	bool mStepped = false;
};
