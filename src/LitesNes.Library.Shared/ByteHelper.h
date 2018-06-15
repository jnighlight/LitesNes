#pragma once
#include <cstdint>

class ByteHelper
{
public:

	ByteHelper() = delete;
	~ByteHelper();

	static bool IsNegative(uint8_t byte);
	static bool IsZero(uint8_t byte);
	static uint8_t AND(uint8_t byte, uint8_t otherByte);
	static uint8_t OR(uint8_t byte, uint8_t otherByte);
	/*
	static void LeftShift(uint8_t byte, uint8_t leftShiftCount);
	static void RightShift(uint8_t byte, uint8_t leftShiftCount);
	*/
};
