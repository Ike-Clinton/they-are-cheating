#pragma once

#include <Windows.h>
#include "offsets.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <algorithm>
#include <iterator>
#include <thread>
#include <chrono>

void printResources(HANDLE phandle, DWORD ADDRESS_RESOURCES_BASE);
void findOffsetInitial(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE);
void findOffsetInGame(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE);
void findOffsetCustom(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE, DWORD dwGold, DWORD dwWood, DWORD dwStone, DWORD dwIron, DWORD dwOil);
