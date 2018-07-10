#pragma once

#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class OAM
{
public:
	struct OAMEntry
	{
		uint8_t yPos = 0;
		uint8_t tileIndex = 0;
		uint8_t attributes = 0;
		uint8_t xPos = 0;
	};

	OAM();
	OAM(std::string name);
	~OAM();
	OAMEntry& GetEntry(uint32_t entry) { return mEntries[entry]; };
	std::vector<OAM::OAMEntry> GetCollidingSprites(uint32_t lineNum);
	void LoadAllOAM(uint8_t* startLoadLocation); // Assumes that startLoadLocation is valid for 64*4 bytes

private:
	OAMEntry mEntries[64]; //May have to be static
};