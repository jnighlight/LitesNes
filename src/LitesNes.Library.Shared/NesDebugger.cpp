#include "pch.h"
#include "NesDebugger.h"
#include "imgui.h"
#include "PPU.h"

StatusRegister NesDebugger::sStatusReg = StatusRegister("S");
Register NesDebugger::mAReg = Register("A");
Register NesDebugger::mXReg = Register("X");
Register NesDebugger::mYReg = Register("Y");
Register NesDebugger::mPCReg = Register("PC");
Register NesDebugger::mSPReg = Register("SP");
uint8_t* NesDebugger::mStack = nullptr;
uint16_t NesDebugger::mNMIVector = 0;
uint16_t NesDebugger::mResetVector = 0;
uint16_t NesDebugger::mIRQVector = 0;
uint32_t NesDebugger::mCpuTime = 0;
bool NesDebugger::sDebug = true;
bool NesDebugger::sPlayer1Strobe = true;
uint8_t NesDebugger::sPlayer1Button = 0;
bool NesDebugger::sPlayer2Strobe = true;
uint8_t NesDebugger::sPlayer2Button = 0;

uint32_t NesDebugger::mActiveInstruction = 0;
std::vector<CpuInstruction> NesDebugger::mInstructionList;
std::vector<std::string> NesDebugger::mInstructionDescriptionList;

Ram NesDebugger::mRam = Ram("Ram");
std::map<uint16_t, uint16_t> NesDebugger::sOperations;

NesDebugger::NesDebugger()
{
	time_t seconds = time(NULL);
	std::stringstream ss;
	ss << seconds;
	sCpuFilename.append("../../");
	sCpuFilename.append(ss.str());
	sCpuFilename.append(".txt");
	mSPReg.Set(0xFD);
}

NesDebugger::~NesDebugger()
{
}

void NesDebugger::Update()
{
	if (mRunning) {
		//for (uint32_t i = 0; i < 2000; ++i)
		while (!IncrementActiveInstruction()) //Timing
		{
			//IncrementActiveInstruction(); //timing
			mStepped = true;
			if (mInstructionList[mActiveInstruction].GetIsBreakpoint())
			{
				mRunning = false;
				mRam.UpdateCachedRamString();
				PPU::VRam.UpdateCachedRamString();
				break;
			}
		}
	}
}

void NesDebugger::LogInstruction(CpuInstruction & instruction)
{
	debugFile.open (sCpuFilename, std::ios::out | std::ios::app);
	std::string outString;
	instruction.GetArgumentDescription(outString);

	std::string argDesc;
	argDesc.append(" ");
	argDesc.append(instruction.GetOperationName());
	argDesc.append(" ");
	argDesc.append(outString);
	uint32_t strSize = uint32_t(argDesc.length());

	debugFile << std::hex << instruction.GetAddress() << "  " << instruction.GetPrettyHexRep() << "   " << argDesc;
	for (uint32_t i = 0; i < 30 - strSize; ++i)
	{
		debugFile << " ";
	}
	char hexes[5];
	NesDebugger::PopulateCharBufferWithHex(hexes, NesDebugger::mAReg.GetRegisterContents());
	debugFile << " A:" << hexes;
	NesDebugger::PopulateCharBufferWithHex(hexes, NesDebugger::mXReg.GetRegisterContents());
	debugFile << " X:" << hexes;
	NesDebugger::PopulateCharBufferWithHex(hexes, NesDebugger::mYReg.GetRegisterContents());
	debugFile << " Y:" << hexes;
	NesDebugger::PopulateCharBufferWithHex(hexes, NesDebugger::sStatusReg.GetRegisterContents());
	debugFile << " P:" << hexes;
	debugFile << " CYC:" << "XXX";
	debugFile << "\n";
	debugFile.close();
}


bool NesDebugger::IncrementActiveInstruction()
{
	//LogInstruction(mInstructionList[mActiveInstruction]);
	if (mInstructionList[mActiveInstruction].GetAddress() == 0xf4ed) // Donkey Kong
	//if (mInstructionList[mActiveInstruction].GetAddress() == 0xc427) // Tennis - not working
	//if (false)
	{
		CpuInstruction::NMIInterrupt();
		return true;
	} else {
		mInstructionList[mActiveInstruction].Execute();
		mActiveInstruction++;
		if (mActiveInstruction >= mInstructionList.size())
		{
			mActiveInstruction = 0;
		}
	}
	mPCReg.Set(uint8_t(mInstructionList[mActiveInstruction].GetAddress()));
	if (!mRunning)
	{
		mRam.UpdateCachedRamString();
		PPU::VRam.UpdateCachedRamString();
	}
	return false;
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

void NesDebugger::PopulateCharBufferWithHexNonTerm(char* buf, uint8_t byteData)
{
	unsigned char firstNibbleLower = (byteData >> 4);
	unsigned char secondNibbleLower = (byteData & 0x0F);
	buf[0] = NibbleToChar(firstNibbleLower);
	buf[1] = NibbleToChar(secondNibbleLower);
	buf[2] = ' ';
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

void NesDebugger::PushByteToStack(uint8_t byte)
{
	NesDebugger::mStack[NesDebugger::mSPReg.GetRegisterContents()] = byte;
	NesDebugger::mSPReg.Subtract(1);
}

uint8_t NesDebugger::PopByteFromStack()
{
	NesDebugger::mSPReg.Add(1);
	uint8_t popped = NesDebugger::mStack[NesDebugger::mSPReg.GetRegisterContents()];
	return popped;
}

void NesDebugger::SetStrobe(NesDebugger::Player player, bool strobe)
{
	if (player == PLAYER1)
	{
		sPlayer1Strobe = strobe;
		if (sPlayer1Strobe)
		{
			sPlayer1Button = 0;
		}
	}
}

uint8_t NesDebugger::GetPlayerButton(NesDebugger::Player player)
{
	uint8_t retVal = 0;
	if (player == PLAYER1)
	{
		if (sPlayer1Strobe)
		{
			if (GetKeyState('A') & 0x8000/*check if high-order bit is set (1 << 15)*/)
			{
				retVal |= 1;
			}
		}
		else
		{
			switch (sPlayer1Button)
			{
			case 0:
				if (GetKeyState('A') & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 1:
				if (GetKeyState('B') & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 2:
				if (GetKeyState(VK_CONTROL) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 3:
				if (GetKeyState(VK_RETURN) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 4:
				if (GetKeyState(VK_UP) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 5:
				if (GetKeyState(VK_DOWN) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 6:
				if (GetKeyState(VK_LEFT) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			case 7:
				if (GetKeyState(VK_RIGHT) & 0x8000/*check if high-order bit is set (1 << 15)*/)
				{
					retVal |= 1;
				}
				++sPlayer1Button;
				break;
			default:
				retVal |= 1;
				break;
			}
		}
	}
	else
	{

	}
	return retVal;
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
	if (ImGui::Button("Stop"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
	{
		mRunning = false;
		mRam.UpdateCachedRamString();
		PPU::VRam.UpdateCachedRamString();
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
	if (ImGui::Button("NMI Interrupt"))
	{
		CpuInstruction::NMIInterrupt();
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
	ImGui::SameLine();
	mSPReg.Render();


	ImVec2 size(50, 20);
	ImVec2 totalListSize = size;
	totalListSize.x += totalListSize.x * 7;
	totalListSize.y *= 15;
	size.x *= 4;
	size.y /= 1.5;
	if (!mRunning)
	{
		if (ImGui::ListBoxHeader("##Instructions", totalListSize))
		{
			std::string outString;
			outString.reserve(30);
			for (uint16_t i = 0; i < mInstructionList.size(); ++i) {
				if (ImGui::Selectable(mInstructionDescriptionList[i].c_str(), i == mActiveInstruction && !mRunning, 0, size))
				{
					mInstructionList[i].ToggleIsBreakpoint();
					if (mInstructionList[i].GetIsBreakpoint()) {
						mInstructionDescriptionList[i][0] = '-';
						mInstructionDescriptionList[i][1] = '>';
					}
					else {
						mInstructionDescriptionList[i][0] = ' ';
						mInstructionDescriptionList[i][1] = ' ';
					}
				}
				if (mStepped && i == mActiveInstruction) {
					ImGui::SetScrollHere(0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
					mStepped = false;
				}
			}
		}
		ImGui::ListBoxFooter();
	}
	ImGui::End();
}

void NesDebugger::CreateInstructionDescription()
{
	std::string outString;
	outString.reserve(30);
	for (uint16_t i = 0; i < mInstructionList.size(); ++i) {
		outString.clear();
		/*
		if (mInstructionList[i].GetIsBreakpoint()) {
			outString.append("->");
		} else {*/
			outString.append("  ");
		//}
		//outString.append(mInstructionList[i].mMemoryLocationBuf);
		std::stringstream ss;
		ss << std::hex << std::setw(4) << std::setfill('0') << mInstructionList[i].GetAddress();
		outString.append(ss.str());
		outString.append("\t");
		outString.append(mInstructionList[i].GetOperationName());
		outString.append("\t");
		mInstructionList[i].GetArgumentDescription(outString);
		/*
		if (ImGui::Selectable(outString.c_str(), i == mActiveInstruction && !mRunning, 0, size))
		{
			mInstructionList[i].ToggleIsBreakpoint();
		}
		if (mStepped && i == mActiveInstruction) {
			ImGui::SetScrollHere(0.25f); // 0.0f:top, 0.5f:center, 1.0f:bottom
			mStepped = false;
		}*/
		NesDebugger::mInstructionDescriptionList.push_back(outString);
	}
}

void NesDebugger::RenderMemoryWindow()
{
	ImGui::Begin("LitesNesMemory");
	mRam.Render();
	ImGui::End();
	ImGui::Begin("LitesNesVRAM");
	PPU::VRam.Render();
	ImGui::End();
}

void NesDebugger::RenderStackWindow()
{
	ImGui::Begin("Stack");
	ImGui::BeginGroup();
	uint32_t index = 0;
	char text[5];
	uint32_t lineLength = 4;
	for (uint32_t line = 0; line < 256; line += lineLength)
	{
		ImGui::Text("RAM %04X", line);
		ImGui::SameLine();
		ImGui::Text("\t");
		for (uint32_t i = 0; i < lineLength; ++i) {
			ImGui::SameLine();
			NesDebugger::PopulateCharBufferWithHex(text, mStack[index]);
			++index;
			ImGui::Text(text);
		}
	}
	ImGui::EndGroup();
	ImGui::End();
}

void NesDebugger::Init()
{
	CreateInstructionDescription();
	NesDebugger::mStack = mRam.GetRamPtr() + 0x100;
	mNMIVector = mRam.GetMemoryByLocation(0xFFFB);
	mNMIVector = mNMIVector << 8;
	mNMIVector = mNMIVector | mRam.GetMemoryByLocation(0xFFFA);

	mResetVector = mRam.GetMemoryByLocation(0xFFFD);
	mResetVector = mResetVector << 8;
	mResetVector = mResetVector | mRam.GetMemoryByLocation(0xFFFC);

	mIRQVector = mRam.GetMemoryByLocation(0xFFFF);
	mIRQVector = mIRQVector << 8;
	mIRQVector = mIRQVector | mRam.GetMemoryByLocation(0xFFFE);

	auto result = NesDebugger::sOperations.find(mResetVector);
	assert(result != NesDebugger::sOperations.end());
	NesDebugger::mActiveInstruction = result->second;
}

void NesDebugger::DrawFromBuffer(uint32_t* texArray)
{
	uint16_t index = 1024;
	uint8_t drawX = mRam.GetMemoryByLocation(index);
	uint8_t drawColor = 0;
	uint8_t drawY = 0;
	while (drawX != 0)
	{
		++index;
		drawY = mRam.GetMemoryByLocation(index);
		++index;
		drawColor = mRam.GetMemoryByLocation(index);
		++index;
		uint32_t texArrayIndex = drawX + (256 * drawY);
		switch (drawColor % 5)
		{
		case 0:
			texArray[texArrayIndex] = 0x000000FF;
			break;
		case 1:
			texArray[texArrayIndex] = 0xFFFFFFFF;
			break;
		case 2:
			texArray[texArrayIndex] = 0xFF0000FF;
			break;
		case 3:
			texArray[texArrayIndex] = 0x00FF00FF;
			break;
		case 4:
			texArray[texArrayIndex] = 0x0000FFFF;
			break;
		default:
			break;
		}
		drawX = mRam.GetMemoryByLocation(index);
	}
}
