#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "OAM.h"
#include "Ram.h"
#include "Nametable.h"
#include "PatternTables.h"
#include "AddressRegister.h"
#include "WritingRegister.h"
#include "OAMRegister.h"

class PPU
{
public:
	static Register PPUCTRL;
	static Register PPUMASK;
	static Register PPUSTATUS;
	static AddressRegister OAMADDR;
	static OAMRegister OAMDATA;
	static AddressRegister PPUSCROLL;
	static AddressRegister PPUADDR;
	static WritingRegister PPUDATA;
	static Register OAMDMA;
	static Ram VRam;
	static OAM sOAM; //Held on the internal PPU memory

	PPU(uint32_t chrRomSize, char* chrRomPtr);
	~PPU();

	//Nametable mNametables; //2KB of VRAM on NES
	PatternTables mPatternTables; //Stored on the cartridge, not the internal PPU VRAM

	void Render();
	void RenderLine(uint32_t lineNum);

	void DrawFromBuffer(uint32_t* texArray);
	void RenderPatternTables(uint32_t* texArray);
	void RenderNametables(uint32_t* texArray);
	void DrawPatternTable(uint32_t* texArray, PatternTables::PatternTable& tableToDraw, uint32_t xOffset, uint32_t yOffset);

	static bool mRenderPatternTables;
	static bool mTableSide;
private:
};
