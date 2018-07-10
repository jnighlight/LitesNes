#pragma once
#include <vector>
#include <iomanip>
#include <sstream>
#include <cstddef>

class RomReader
{
public:

	RomReader();
	RomReader(std::string name);
	~RomReader();
	void ReadFile();

	void Clear();
	void SetFilename(std::string filename) { mFileName = filename; };
	bool GetIsPopulated() { return mIsPopulated; };

	char* GetPrgRomData() { return mPrgRomData; };
	uint32_t GetPrgRomDataSize() { return mPrgRomDataSize; };
	char* GetChrRomData() { return mChrRomData; };
	uint32_t GetChrRomDataSize() { return mChrRomDataSize; };

private:
	std::string mFileName = "";
	bool mIsPopulated = false;

	char mHeader[16];
	char* mPrgRomData = nullptr;
	uint32_t mPrgRomDataSize = 0;
	char* mChrRomData = nullptr;
	uint32_t mChrRomDataSize = 0;
};
