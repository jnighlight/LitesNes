#pragma once

#include <vector>
#include <iomanip>
#include <sstream>

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
		NesInstruction(uint32_t memLoc)
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
	void Render();
	
	std::vector<NesInstruction> mInstructionList;
	uint32_t mActiveInstruction = 0;
private:
	char mARegBuf[8] = "";
	uint16_t mAReg = 0;

	char mXRegBuf[8] = "";
	uint16_t mXReg = 0;

	char mYRegBuf[8] = "";
	uint16_t mYReg = 0;

	char mPCRegBuf[8] = "";
	uint16_t mPCReg = 0;
};

