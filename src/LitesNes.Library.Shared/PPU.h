#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "OAM.h"
#include "Ram.h"
#include "Nametable.h"
#include "PatternTables.h"
#include "Register.h"

class PPU
{
public:
	static Register PPUCTRL;
	static Register PPUMASK;
	static Register PPUSTATUS;
	static Register OAMADDR;
	static Register OAMDATA;
	static Register PPUSCROLL;
	static Register PPUADDR;
	static Register PPUDATA;
	static Register OAMDMA;
	static Ram VRam;

	PPU(uint32_t chrRomSize, char* chrRomPtr);
	~PPU();

	OAM mOAM; //Held on the internal PPU memory
	Nametable mNametables; //2KB of VRAM on NES
	PatternTables mPatternTables; //Stored on the cartridge, not the internal PPU VRAM

	void Render();
	void RenderLine(uint32_t lineNum);
	void DrawFromBuffer(uint32_t* texArray);
private:
};
