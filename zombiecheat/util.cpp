#include "stdafx.h"
#include "util.h"
BYTE GetFirstByte(DWORD* dword) 
{
	return (BYTE)(*dword >> 24);
}
BYTE GetSecondByte(DWORD* dword) 
{
	return (BYTE)(*dword << 8 >> 24);
}
BYTE GetThirdByte(DWORD* dword) 
{
	return (BYTE)(*dword << 16 >> 24);
}
BYTE GetFourthByte(DWORD* dword) 
{
	return (BYTE)(*dword << 24 >> 24);
}