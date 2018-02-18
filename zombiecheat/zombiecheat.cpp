// zombiecheat.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include <iostream>
#include "offsets.h"

using namespace std;

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

	// Read Gold value
	ReadProcessMemory(phandle, (void*)ADDRESS_GOLD, &dwGold, sizeof(dwGold), 0);
	cout << "Gold: " << dwGold << "\n";

	// Read Wood1 value
	ReadProcessMemory(phandle, (void*)ADDRESS_WOOD1, &dwWood, sizeof(dwWood), 0);
	cout << "Wood1: " << dwWood << "\n";

	// Read wood2 value
	ReadProcessMemory(phandle, (void*)ADDRESS_WOOD2, &dwWood, sizeof(dwWood), 0);
	cout << "Wood2: " << dwWood << "\n";

	while(1) {
		SIZE_T written;
		DWORD dwValue = 0x00000000;
		DWORD choice = 0;
		cout << "Please enter the param you would like to increase: \n\n";
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
				WriteProcessMemory(phandle, (void*)ADDRESS_WOOD1, &dwValue, sizeof(dwValue), &written);
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

int readAllValues() {

	return 0;
}

