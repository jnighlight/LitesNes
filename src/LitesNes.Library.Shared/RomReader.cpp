#include "pch.h"
#include "RomReader.h"
#include <ctype.h>
#include <iostream>
#include <fstream>

RomReader::RomReader()
{}

RomReader::RomReader(std::string filename)
	:mFileName(filename)
{
}

void RomReader::Clear()
{
	if (mPrgRomData != nullptr)
	{
		delete mPrgRomData;
		mPrgRomData = nullptr;
		mPrgRomDataSize = 0;
	}
	if (mChrRomData != nullptr)
	{
		delete mChrRomData;
		mChrRomData = nullptr;
		mChrRomDataSize = 0;
	}
	mIsPopulated = false;
}

void RomReader::ReadFile()
{
	if (mFileName.empty()) {
		throw std::exception("Attempting to open empty filename");
	}
	if (mIsPopulated) { Clear(); };
	std::ifstream romFile;
	romFile.open(mFileName, std::ios::in | std::ios::binary);
	if (!romFile.is_open())
	{
		romFile.close(); //Just in case
		throw std::exception("Can't open file!");
	}
	romFile.read(mHeader, 16);
	uint8_t prgRom16kbCount = mHeader[4];
	uint8_t chrRom8kbCount = mHeader[5];
	mPrgRomDataSize = 16384 * prgRom16kbCount;
	mChrRomDataSize = 8192 * chrRom8kbCount;
	mPrgRomData = new char[mPrgRomDataSize];
	mChrRomData = new char[mChrRomDataSize];
	romFile.read(mPrgRomData, mPrgRomDataSize);
	romFile.read(mChrRomData, mChrRomDataSize);
	mIsPopulated = true;
	romFile.close(); //Just in case
}

RomReader::~RomReader()
{
}
