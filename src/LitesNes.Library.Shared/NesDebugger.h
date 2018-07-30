#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>
#include <stack>
#include <fstream>
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
	
	enum Player
	{
		PLAYER1 = 0,
		PLAYER2
	};

	NesDebugger();
	~NesDebugger();
	void RenderDebugger();
	void RenderMemoryWindow();
	void RenderStackWindow();
	void Init();
	void Update();
	void DrawFromBuffer(uint32_t* texArray);
	static void PopulateCharBufferWithHex(char* buf, uint16_t byteData);
	static void PopulateCharBufferWithHex(char * buf, uint8_t byteData);
	static void PopulateCharBufferWithHex(char * buf, std::byte byte);
	static void PopulateCharBufferWithHexNonTerm(char* buf, uint8_t byteData);
	static void PushByteToStack(uint8_t byte);
	static uint8_t PopByteFromStack();
	static uint8_t GetPlayerButton(NesDebugger::Player player);
	static void SetStrobe(NesDebugger::Player player, bool strobe);

	static char NibbleToChar(std::byte nybble);
	
	static uint32_t mActiveInstruction;
	static StatusRegister sStatusReg;// = StatusRegister("S");
	static Register mAReg;// = Register("A");
	static Register mXReg;// = Register("X");
	static Register mYReg;// = Register("Y");
	static Register mPCReg;// = Register("PC");
	static Register mSPReg;// = Register("SP");
	static uint8_t* mStack;
	static uint16_t mNMIVector;
	static uint16_t mResetVector;
	static uint16_t mIRQVector;
	static uint32_t mCpuTime;
	static bool sDebug;
	static bool sPlayer1Strobe;
	static uint8_t sPlayer1Button;
	static bool sPlayer2Strobe;
	static uint8_t sPlayer2Button;

	static Ram mRam;// = Ram("Ram");
	static std::vector<CpuInstruction> mInstructionList;
	static std::vector<std::string> mInstructionDescriptionList;

	static std::map<uint16_t, uint16_t> sOperations;

private:
	bool IncrementActiveInstruction();
	void CreateInstructionDescription();
	static char NibbleToChar(unsigned char nybble);
	void LogInstruction(CpuInstruction& instruction);

	bool mRunning = false;
	bool mShouldStep = false;
	bool mStepped = false;
	std::string sCpuFilename;
	std::ofstream debugFile;
};
