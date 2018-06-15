#include "pch.h"
#include "ByteHelper.h"


bool ByteHelper::IsNegative(uint8_t byte)
{
	return byte & 0b10000000;
}

bool ByteHelper::IsZero(uint8_t byte)
{
	return byte == 0;
}

uint8_t ByteHelper::AND(uint8_t byte, uint8_t otherByte)
{
	return byte & otherByte;
}

uint8_t ByteHelper::OR(uint8_t byte, uint8_t otherByte)
{
	return byte | otherByte;
}
