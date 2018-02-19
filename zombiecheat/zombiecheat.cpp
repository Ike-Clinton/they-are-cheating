// zombiecheat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "main.h"

using namespace std;
ofstream ofile;

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

	std::string mypattern = "0000003200000032";


	//Store system info in struct SYSTEM_INFO
	SYSTEM_INFO sysInfo;
	GetNativeSystemInfo(&sysInfo);
	const DWORD PAGESIZE = sysInfo.dwPageSize;
	cout << "\nPAGESIZE = " << PAGESIZE << "\n";

	#define ADDRESS1 0x07182578
	
	BYTE buffer[16];
	ReadProcessMemory(phandle, (void*)ADDRESS1, &buffer, sizeof(BYTE)*16, 0);
	//printf("%032X", buffer);
	for (int i = 0; i < sizeof(buffer); i++ ) {
		printf("%02X", buffer[i]);
	}

	// Find the locations, and write them to standard output, one per line:
	//find_locs(phandle, mypattern, std::ostream_iterator<void *>(std::cout, "\n"));

	//char *base = (char *)0;
	//do {

	//	char** buffer = new char*[PAGESIZE];

	//	if (ReadProcessMemory(phandle, base, buffer, PAGESIZE, NULL) != 0)
	//	{
	//		for (int i = 0; i < PAGESIZE-1; i += sizeof(DWORD)) {
	//			cout << "\n" << i;
	//			printf("%02X", buffer[i]);
	//		}
	//	}

	//	base += PAGESIZE;

	//	delete[] buffer;

	//	// keep looping going until we wrap back around to 0
	//} while (base != 0);



	// Read Gold value
	//ReadProcessMemory(phandle, (void*)ADDRESS_GOLD, &dwGold, sizeof(dwGold), 0);
	//cout << "Gold: " << dwGold << "\n";

	// Read Wood1 value
	//ReadProcessMemory(phandle, (void*)ADDRESS_WOOD1, &dwWood, sizeof(dwWood), 0);
	//cout << "Wood1: " << dwWood << "\n";

	while (1) {
		SIZE_T written;
		DWORD dwValue = 0x00000000;
		DWORD choice = 0;
		cout << "\nPlease enter the param you would like to increase: \n";
		cout << "(1) Wood\n";
		cout << "(2) Stone\n";
		cout << "(3) Iron\n";
		cout << "(4) Oil\n";
		cout << "(5) Gold\n";
		cout << "(6) Food\n";
		cout << "(7) Workers\n";
		cout << "(8) Power\n";
		cin >> choice;
		switch (choice) {
			// Wood
		case 1:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_WOOD, &dwValue, sizeof(dwValue), &written);
			break;
			// Stone
		case 2:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_STONE, &dwValue, sizeof(dwValue), &written);
			break;
			// Iron
		case 3:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_IRON, &dwValue, sizeof(dwValue), &written);
			break;
			// Oil
		case 4:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_OIL, &dwValue, sizeof(dwValue), &written);
			break;
			// Gold
		case 5:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_GOLD, &dwValue, sizeof(dwValue), &written);
			break;
			// Food
		case 6:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_FOOD, &dwValue, sizeof(dwValue), &written);
			break;
			// Workers
		case 7:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_WORKERS, &dwValue, sizeof(dwValue), &written);
			break;
			// Power
		case 8:
			cout << "\n Enter the amount: ";
			cin >> dwValue;
			WriteProcessMemory(phandle, (void*)ADDRESS_POWER, &dwValue, sizeof(dwValue), &written);
			break;
		default:
			cout << "Please enter a valid choice\n";
			break;

		}
	}

	return 0;
}

template <class outIter>
void find_locs(HANDLE process, std::string const &pattern, outIter output) {

	unsigned char *p = NULL;
	MEMORY_BASIC_INFORMATION info;

	// VirtualQueryEx does the hard part, telling use about each
	// block in the target process.

	for (p = NULL;
		VirtualQueryEx(process, p, &info, sizeof(info)) == sizeof(info);
		p += info.RegionSize)
	{
		// buffer to hold copies of memory blocks from the target
		std::vector<char> buffer;
		std::vector<char>::iterator pos;



		// We only want committed memory that's mapped or private -- 
		// screens out things like the code in the target process.
		// 
		if (info.State == MEM_COMMIT &&
			(info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE))
		{
			SIZE_T bytes_read;

			// copy block from target to our buffer, search for pattern:
			buffer.resize(info.RegionSize);
			ReadProcessMemory(process, p, &buffer[0], info.RegionSize, &bytes_read);
			buffer.resize(bytes_read);



			// find all occurrences of pattern in buffer:
			for (pos = buffer.begin();
				buffer.end() != (pos = std::search(pos, buffer.end(), pattern.begin(), pattern.end()));
				++pos)
			{
				// record each address in target where pattern was found:
				*output++ = p + (pos - buffer.begin());
			}
		}
	}
}

void findBytes(HANDLE phandle, BYTE* bytes, int length) {

}
