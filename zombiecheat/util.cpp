#include "stdafx.h"
#include "util.h"
BYTE GetFirstByte(DWORD dword) 
{
	return (BYTE)(dword >> 24) & 0xFF;
}
BYTE GetSecondByte(DWORD dword) 
{
	return (BYTE)(dword >>16) & 0xFF;
}
BYTE GetThirdByte(DWORD dword) 
{
	return (BYTE)(dword >> 8) & 0xFF;
}
BYTE GetFourthByte(DWORD dword) 
{
	return (BYTE)(dword & 0xFF);
}