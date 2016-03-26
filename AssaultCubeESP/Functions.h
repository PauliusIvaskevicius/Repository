#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>
#include <cmath>

using namespace std;



DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	DWORD pTemp;
	DWORD pointerAddr;
	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, sizeof(pTemp), NULL);
		}
		pointerAddr = pTemp + Offsets[i];
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, sizeof(pTemp), NULL);
	}
	return pointerAddr;
}

void FindWindowTool(HWND& hGameWindow, HANDLE& hProcHandle, DWORD& dwProcID,string& GameStatus, LPCSTR LGameWindow)
{
	hGameWindow = FindWindowA(NULL, LGameWindow);
	if (hGameWindow)
	{
		GetWindowThreadProcessId(hGameWindow, &dwProcID);
		if (dwProcID != 0)
		{
			hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
			if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == false)
			{
				GameStatus = "Failed to open process";
			}
			else
			{
				GameStatus = "Ready to cheat";
			}
		}
		else
		{
			GameStatus = "Failed to get process ID";
		}
	}
	else
	{
		GameStatus = "Failed to find window";
	}

}



//GDE
