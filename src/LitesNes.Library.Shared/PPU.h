#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

#include "OAM.h"
#include "Ram.h"

class PPU
{
public:

	PPU();
	PPU(std::string name);
	~PPU();

	OAM mOAM; //Held on the internal PPU memory
	Ram mVRam;
	Ram mPatternTables; //Stored on the cartridge, not the internal PPU VRAM

private:
};
