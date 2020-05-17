
#include "pch.h"
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
//sauerbraten.exe+213EA8

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}


int main()
{


	class Vector3 {
	public:
		float x, y, z;

	};

	class Vector2 {
	public:
		float x, y;
	};

	
	class PlayerBase
	{
	public:
		Vector3 N0000004F; //0x0000
		char pad_000C[344]; //0x000C
		int32_t N000000A7; //0x0164
		char pad_0168[736]; //0x0168
	}; //Size: 0x0448

	DWORD pistolAmmo = 0x00EBAD68;
	DWORD pid;
	HANDLE procHandle;
	HWND WinHWND = FindWindowA(NULL,"Cube 2: Sauerbraten");
	GetWindowThreadProcessId(WinHWND,&pid);
	uintptr_t playerBase = GetModuleBaseAddress(pid, L"sauerbraten.exe")+0x213EA8;
	uintptr_t healthAddress = playerBase + 0x15c;
	if (pid) {
		printf("PID Found: %d\n",pid);
		procHandle = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

		if (procHandle) {
			int newAmmo = 999;
			int newHealth = 9999;
			printf("Handle gotten: %d\n", procHandle);
			printf("Playerbase Gotten: %d\n", playerBase);
			WriteProcessMemory(procHandle, (void*)pistolAmmo, &newAmmo, sizeof(newAmmo), NULL);
			ReadProcessMemory(procHandle, (void*)playerBase, &playerBase, sizeof(playerBase), NULL);
			WriteProcessMemory(procHandle, (void*)(playerBase+0x15c), &newHealth, sizeof(newHealth), NULL);
			printf("Error: %d", GetLastError());
		}
		else {
			printf("Unable to recieve handle");

		}
	}
	scanf_s("Quit");
}
