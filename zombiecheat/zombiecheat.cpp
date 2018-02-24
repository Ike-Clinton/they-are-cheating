#include "stdafx.h"
#include "main.h"

// TODO create resources struct

//TODO split this into classes...

using namespace std;


const DWORD OFFSET_FOOD = 0x00000000;
const DWORD OFFSET_WORKERS = 0x00000000;
const DWORD OFFSET_POWER = 0x00000000;

int main()
{
	DWORD dwWood = 0x00000000;
	DWORD dwStone = 0x00000000;
	DWORD dwIron = 0x00000000;
	DWORD dwOil = 0x00000000;
	DWORD dwGold = 0x00000000;
	DWORD dwFood = 0x00000000;
	DWORD dwWorkers = 0x00000000;
	DWORD dwPower = 0x00000000;

	DWORD ADDRESS_RESOURCES_BASE = 0x00000000;


	// Get HWND based on game title
	DWORD dwPid;
	HWND hwnd;
	hwnd = FindWindow(NULL, L"They Are Billions");
	if (!hwnd) {
		cout << "Window not found, FindWindow returned NULL\n";
		return 0;
	}
	// Get process ID, set it at address of dwPid
	GetWindowThreadProcessId(hwnd, &dwPid);
	DWORD dwAccess = PROCESS_VM_READ |
		PROCESS_VM_WRITE |
		PROCESS_VM_OPERATION |
		PROCESS_QUERY_INFORMATION;
	// Open a handle to the process specified by pid
	HANDLE phandle = OpenProcess(dwAccess, 0, dwPid);
	// Exit if the above failed
	if (!phandle) {
		cout << "Couldn't attach to process. OpenProcess returned error\n";
		return 0;
	}
	else {
		cout << "Attached to process with pid " << dwPid << "\n";
	}

	cout << "\n";
	int choice = 0;
	while (choice == 0){
		printf("Please select an option: \n");
		printf("(1) New game (default resources)\n");
		printf("(2) Cheated game (hacked resources standard)\n");
		printf("(3) Custom starting resources\n");
		cin >> choice;

		switch (choice) {
		case 1:
			// Find initial resource offset
			findOffsetInitial(phandle, &ADDRESS_RESOURCES_BASE);
			printResources(phandle, ADDRESS_RESOURCES_BASE);
			break;
		case 2:
			// Find hacked game resources offset
			findOffsetInGame(phandle, &ADDRESS_RESOURCES_BASE);
			printResources(phandle, ADDRESS_RESOURCES_BASE);
			break;
		case 3:
			// Get custom values and find offset
			printf("Please enter Gold, Wood, Stone, Iron, Oil values");
			cin >> dwGold;
			cin >> dwWood;
			cin >> dwStone;
			cin >> dwIron;
			cin >> dwOil;
			findOffsetCustom(phandle, &ADDRESS_RESOURCES_BASE, dwGold, dwWood, dwStone, dwIron, dwOil);
			printResources(phandle, ADDRESS_RESOURCES_BASE);
			break;
		default:
			choice = 0;
			break;

		};
	}
	

	while (1) {
		
		SIZE_T written;
		DWORD dwValue = 0x0FF00000;

		DWORD dwWood = 0x00030D40;
		DWORD dwStone = 0x00030D40;
		DWORD dwIron = 0x00030D40;
		DWORD dwOil = 0x00030D40;
		DWORD dwGold = 0x007A0012;

		// if not still same value, find offsets again

		// Write our resources
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_GOLD), &dwValue, sizeof(dwGold), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_WOOD), &dwValue, sizeof(dwWood), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_STONE), &dwValue, sizeof(dwStone), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_IRON), &dwValue, sizeof(dwIron), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_OIL), &dwValue, sizeof(dwOil), &written);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	return 0;
}


/*
 * Print player's current resources
 * TODO: Pass in a struct of the resouces
 *
*/
void printResources(HANDLE phandle, DWORD ADDRESS_RESOURCES_BASE) {

	DWORD dwWood = 0x00030D40;
	DWORD dwStone = 0x00030D40;
	DWORD dwIron = 0x00030D40;
	DWORD dwOil = 0x00030D40;
	DWORD dwGold = 0x007A0012;

	DWORD dwFood = 0x00000000;
	DWORD dwWorkers = 0x00000000;
	DWORD dwPower = 0x00000000;

	// Read Gold value
	ReadProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_GOLD), &dwGold, sizeof(dwGold), 0);
	printf("Gold: %d\n", dwGold);

	// Read Wood Value
	ReadProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_WOOD), &dwWood, sizeof(dwWood), 0);
	printf("Wood: %d\n", dwWood);

	// Read Stone value
	ReadProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_STONE), &dwStone, sizeof(dwStone), 0);
	printf("Stone: %d\n", dwStone);

	// Read Iron value
	ReadProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_IRON), &dwIron, sizeof(dwIron), 0);
	printf("Iron: %d\n", dwIron);

	// Read Oil value
	ReadProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_OIL), &dwOil, sizeof(dwOil), 0);
	printf("Oil: %d\n", dwOil);
}

/*!
*  Find the offset of the resources struct for a new game
*
*  @param[in]  phandle Handle to the process for TheyAreBillions.exe
*  @param[in]  ADDRESS_RESOUCES_BASE DWORD ptr 
*  @param[out] pdwOut This is a parameter that is filled in.
*
*  @return Void
*/
void findOffsetInitial(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE) {

	printf("Searching for resource location in memory . . .\n");

	char initialPattern[] = {
		0x14, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x64, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00,
		0x0A, 0x00, 0x00, 0x00,
	};

	//Store system info in struct SYSTEM_INFO
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	const DWORD PAGESIZE = sysInfo.dwPageSize;
	cout << "\nPAGESIZE = " << PAGESIZE << "\n";

	DWORD64 base = (DWORD64)sysInfo.lpMinimumApplicationAddress;
	DWORD64 end = (DWORD64)sysInfo.lpMaximumApplicationAddress;
	printf("Min Address: %I64X\n", base);
	printf("Max Address: %I64X\n", end);
	bool found = false;
	

	do {
		char buffer[4096];
		if (ReadProcessMemory(phandle, (void*)base, buffer, PAGESIZE, NULL) != 0)
		{
			auto it = std::search(
				std::begin(buffer), std::end(buffer),
				std::begin(initialPattern), std::end(initialPattern));

			if (it == std::end(buffer)) {
				//not found
			}
			else
			{
				// We found our signature, get the address of the struct
				*ADDRESS_RESOURCES_BASE = base + std::distance(std::begin(buffer), it);
				printf("Found: %I64X\n", *ADDRESS_RESOURCES_BASE);
				found = true;

			}
		}
		base += PAGESIZE;
		// keep looping going until we wrap back around to 0
	} while (base != end & !found);
}


/*!
*  Find the offset of the resources struct for a game in progress
*
*  @param[in]  phandle Handle to the process for TheyAreBillions.exe
*  @param[in]  ADDRESS_RESOUCES_BASE DWORD ptr
*  @param[out] pdwOut This is a parameter that is filled in.
*
*  @return Void
*/
void findOffsetInGame(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE) {

	printf("Searching for resource location in memory . . .\n");

	char inGamePattern[] = {
		0x40, 0x0D, 0x03, 0x00,
		0x40, 0x0D, 0x03, 0x00,
		0x40, 0x0D, 0x03, 0x00,
		0x40, 0x0D, 0x03, 0x00,
		0x00, 0x12, 0x7A, 0x00,
		0x02, 0x01, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
	};

	//40 0D 03 00 40 0D 03 00 40 0D 03 00 40 0D 03 00 00 12 7A 00

	//Store system info in struct SYSTEM_INFO
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	const DWORD PAGESIZE = sysInfo.dwPageSize;

	// Get the application max and min addresses
	DWORD64 base = (DWORD64)sysInfo.lpMinimumApplicationAddress;
	DWORD64 end = (DWORD64)sysInfo.lpMaximumApplicationAddress;
	printf("Min Address: %I64X\n", base);
	printf("Max Address: %I64X\n", end);
	bool found = false;


	do {
		char buffer[4096];
		if (ReadProcessMemory(phandle, (void*)base, buffer, PAGESIZE, NULL) != 0)
		{
			auto it = std::search(
				std::begin(buffer), std::end(buffer),
				std::begin(inGamePattern), std::end(inGamePattern));

			if (it == std::end(buffer)) {
				//not found
			}
			else
			{
				// We found our signature, get the address of the struct
				*ADDRESS_RESOURCES_BASE = base + std::distance(std::begin(buffer), it);
				printf("Found: %I64X\n", *ADDRESS_RESOURCES_BASE);
				found = true;

			}
		}
		base += PAGESIZE;
		// keep looping going until we wrap back around to 0
	} while (base != end & !found);

}

/*!
*  Find the offset of the resources struct for custom amount of resources
*
*  @param[in]  phandle Handle to the process for TheyAreBillions.exe
*  @param[in]  ADDRESS_RESOUCES_BASE DWORD ptr
*  @param[in]  dwGold Current gold value in game
*  @param[in]  dwWood Current wood value in game
*  @param[in]  dwStone Current stone value in game
*  @param[in]  dwIron Current Iron value in game
*  @param[in]  dwOil Current Oil value in game
*  @param[out] pdwOut This is a parameter that is filled in.
*
*  @return Void
*/
void findOffsetCustom(HANDLE phandle, DWORD* ADDRESS_RESOURCES_BASE, DWORD dwGold, DWORD dwWood, DWORD dwStone, DWORD dwIron, 
	DWORD dwOil) {

	printf("Searching for resource location in memory . . .\n");

	printf("Values:\n\tGold: %16d\n\tWood: ");

	BYTE *lpWood = (BYTE*)&dwWood;
	BYTE *lpStone = (BYTE*)&dwStone;
	BYTE *lpIron = (BYTE*)&dwIron;
	BYTE *lpOil = (BYTE*)&dwOil;
	BYTE *lpGold = (BYTE*)&dwGold;

	char customPattern[] = {
		lpWood[3], lpWood[2], lpWood[1], lpWood[0], // Wood
		lpStone[3], lpStone[2], lpStone[1], lpStone[0], // Stone
		lpIron[3], lpIron[2], lpIron[1], lpIron[0], // Iron
		lpOil[3], lpOil[2], lpOil[1], lpOil[0], // Oil
		lpGold[3], lpGold[2], lpGold[1], lpGold[0], // Gold
		0x00, 0x00, 0x00, 0x00,
		0x32, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00,
		0x14, 0x00, 0x00, 0x00,
		0x0A, 0x00, 0x00, 0x00,
	};

	//40 0D 03 00 40 0D 03 00 40 0D 03 00 40 0D 03 00 00 12 7A 00

	//Store system info in struct SYSTEM_INFO
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	const DWORD PAGESIZE = sysInfo.dwPageSize;

	// Get the application max and min addresses
	DWORD64 base = (DWORD64)sysInfo.lpMinimumApplicationAddress;
	DWORD64 end = (DWORD64)sysInfo.lpMaximumApplicationAddress;
	printf("Min Address: %I64X\n", base);
	printf("Max Address: %I64X\n", end);
	bool found = false;


	do {
		char buffer[4096];
		if (ReadProcessMemory(phandle, (void*)base, buffer, PAGESIZE, NULL) != 0)
		{
			auto it = std::search(
				std::begin(buffer), std::end(buffer),
				std::begin(customPattern), std::end(customPattern));

			if (it == std::end(buffer)) {
				//not found
			}
			else
			{
				// We found our signature, get the address of the struct
				*ADDRESS_RESOURCES_BASE = base + std::distance(std::begin(buffer), it);
				printf("Found: %I64X\n", *ADDRESS_RESOURCES_BASE);
				found = true;

			}
		}
		base += PAGESIZE;
		// keep looping going until we wrap back around to 0
	} while ((base != end) & !found);

}



