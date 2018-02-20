// zombiecheat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"

// TODO create resources struct

using namespace std;

const DWORD OFFSET_WOOD = 0x00;
const DWORD OFFSET_STONE = 0x04;
const DWORD OFFSET_IRON = 0x08;
const DWORD OFFSET_OIL = 0x0C;
const DWORD OFFSET_GOLD = 0x10;
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



	//Store system info in struct SYSTEM_INFO
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	const DWORD PAGESIZE = sysInfo.dwPageSize;
	cout << "\nPAGESIZE = " << PAGESIZE << "\n";

	//#define ADDRESS1 0x07182568
	//BYTE buffer[16];
	//ReadProcessMemory(phandle, (void*)ADDRESS1, &buffer, sizeof(BYTE)*16, 0);

	//for (int i = 0; i < sizeof(buffer); i++ ) {
	//	printf("%02X", buffer[i]);
	//}

	char pattern[] = { 
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
	cout << "\n";

	DWORD64 base = (DWORD64)sysInfo.lpMinimumApplicationAddress;
	DWORD64 end = (DWORD64)sysInfo.lpMaximumApplicationAddress;
	printf("Min Address: %I64X\n", base);
	printf("Max Address: %I64X\n", end);
	bool found = false;

	printf("Searching for resource location in memory . . .\n");

	do {
		char buffer[4096];
		if (ReadProcessMemory(phandle, (void*)base, buffer, PAGESIZE, NULL) != 0)
		{
			auto it = std::search(
				std::begin(buffer), std::end(buffer),
				std::begin(pattern), std::end(pattern));

			if (it == std::end(buffer)) {
				//not found
			}
			else
			{
				// We found our signature, get the address of the struct
				ADDRESS_RESOURCES_BASE = base + std::distance(std::begin(buffer), it);
				printf("Found: %I64X\n", ADDRESS_RESOURCES_BASE);
				found = true;
				
			}
		}
		base += PAGESIZE;
		// keep looping going until we wrap back around to 0
	} while (base != end & !found);

	printResources(phandle, ADDRESS_RESOURCES_BASE);

	while (1) {
		
		SIZE_T written;
		DWORD dwValue = 0x0FF00000;

		// Write our resources
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_GOLD), &dwValue, sizeof(dwValue), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_WOOD), &dwValue, sizeof(dwValue), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_STONE), &dwValue, sizeof(dwValue), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_IRON), &dwValue, sizeof(dwValue), &written);
		WriteProcessMemory(phandle, (void*)(ADDRESS_RESOURCES_BASE + OFFSET_OIL), &dwValue, sizeof(dwValue), &written);

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

	DWORD dwWood = 0x00000000;
	DWORD dwStone = 0x00000000;
	DWORD dwIron = 0x00000000;
	DWORD dwOil = 0x00000000;
	DWORD dwGold = 0x00000000;
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

