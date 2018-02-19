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

template <class outIter>
void find_locs(HANDLE process, std::string const &pattern, outIter output);

