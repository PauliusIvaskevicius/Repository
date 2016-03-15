#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>

using namespace std;

string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
string GameStatus;

bool isGameAvail;
bool UpdateOnNextRun;

//AMMO
bool AmmoStatus;
BYTE AmmoValue[] = { 0xA3, 0x1C,0x0,0x0 };
DWORD AmmoBaseAddress = { 0x028FA410 };
DWORD AmmoOffsets[] = { 0x384,0x14,0x0 };

//HEALTH
bool HealthStatus;
BYTE HealthValue[] = { 0x39, 0x5,0x0,0x0 };
DWORD HealthBaseAddress = { 0x028FA410 };
DWORD HealthOffsets[] = { 0xF8 };

DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress);
void WriteToMemory(HANDLE hProcHandle);

int main()
{
	HWND hGameWindow = NULL;
	HANDLE hProcHandle = NULL;
	DWORD dwProcID = NULL;
	int timeSinceLastUpdate = clock();
	int gameAvailTimer = clock();
	int onePressTimer = clock();
	UpdateOnNextRun = true;
	string sAmmoStatus = "OFF";
	string sHealthStatus = "OFF";


	while (!GetAsyncKeyState(VK_INSERT))
	{
		if (clock() - gameAvailTimer > 100)
		{
			gameAvailTimer = clock();
			isGameAvail = false;

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
						isGameAvail = true;
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

			if (UpdateOnNextRun || clock() - timeSinceLastUpdate > 5000)
			{
				system("cls");
				cout << "-------------------------------------------------------------" << endl;
				cout << "                       Assault Cube Hacker                   " << endl;
				cout << "-------------------------------------------------------------" << endl << endl;
				cout << "Game status : " << GameStatus << endl;
				cout << "[F1] Unlimited Ammo : " << sAmmoStatus << endl;
				cout << "[F2] Unlimited Health  : " << sHealthStatus << endl;
				cout << "[INSERT] Exit  : " << endl;
				UpdateOnNextRun = false;
				timeSinceLastUpdate = clock();
			}

			if (isGameAvail)
			{
				//Write to memory

			}
		}

		if (clock() - onePressTimer > 400)
		{
			if (isGameAvail)
			{
				if (GetAsyncKeyState(VK_F1))
				{
					onePressTimer = clock();
					AmmoStatus = !AmmoStatus;
					UpdateOnNextRun = true;
					if (AmmoStatus)
						sAmmoStatus = "ON";
					else
						sAmmoStatus = "OFF";
				}
				else if (GetAsyncKeyState(VK_F2))
				{
					onePressTimer = clock();
					HealthStatus = !HealthStatus;
					UpdateOnNextRun = true;
					if (HealthStatus)
						sHealthStatus = "ON";
					else
						sHealthStatus = "OFF";
				}

			}
		}
	}
	CloseHandle(hProcHandle);
	CloseHandle(hGameWindow);

	return ERROR_SUCCESS;
}


DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
}

void WriteToMemory(HANDLE hProcHandle)
{
	if (AmmoStatus)
	{
		DWORD AmmoAdressToWrite = FindDmaAddy();
	}

	
}