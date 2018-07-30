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
	struct PPUColor {
		PPUColor(uint8_t r, uint8_t g, uint8_t b)
			:mRed(r), mGreen(g), mBlue(b) {};

		uint8_t mRed = 0;
		uint8_t mGreen = 0;
		uint8_t mBlue = 0;
		uint32_t GetColor()
		{
			return (mRed << 24) | (mGreen << 16) | (mBlue << 8) | 0xFF;
		}
	};
	static PPUColor Colors[0x40];
	static Register PPUCTRL;
	static Register PPUMASK;
	static Register PPUSTATUS;
	static AddressRegister OAMADDR;
	static OAMRegister OAMDATA;
	static AddressRegister PPUSCROLL;
	static AddressRegister PPUADDR;
	static WritingRegister PPUDATA;
	static OAMRegister OAMDMA;
	static Ram VRam;
	static OAM sOAM; //Held on the internal PPU memory

	PPU(uint32_t chrRomSize, char* chrRomPtr);
	~PPU();

	PatternTables mPatternTables; //Stored on the cartridge, not the internal PPU VRAM

	void Render(uint32_t* texArray);
	void RenderLine(uint32_t lineNum, uint32_t* texArray);

	void DrawFromBuffer(uint32_t* texArray);
	void RenderPatternTables(uint32_t* texArray);
	void RenderNametables(uint32_t* texArray);
	void DrawPatternTable(uint32_t* texArray, PatternTables::PatternTable& tableToDraw, uint32_t xOffset, uint32_t yOffset);
	void DrawBackground(uint32_t lineNum, uint32_t* lineToDrawTo);
	void DrawSpriteParts(uint32_t lineNum, uint32_t* lineToDrawTo, OAM::OAMEntry& oamEntry);
	//void GetPalette(uint16_t row, uint8_t column);


	static bool mRenderPatternTables;
	static bool mTableSide;
	const static uint32_t ScreenWidth = 256;
	const static uint32_t ScreenHeight = 240;
private:
};
