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
