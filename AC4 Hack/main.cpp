#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>

using namespace std;

string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
string GameStatus;

bool isGameAvail;
bool UpdateAmmoOnNextRun;

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


int main()
{

	return 0;
}