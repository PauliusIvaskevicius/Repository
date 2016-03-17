#include <Windows.h>
#include <ctime>
#include <string>
#include <iostream>
//extern int UIMain();
using namespace std;

void WriteToMemory(DWORD addressToWrite, char* valueToWrite, int byteNum);
DWORD FindDmaAddy(int PointerLevel, DWORD Offsets[], DWORD BaseAddress); 
string Nops[] =
{
	"\x90",
	"\x90","\x90",
	"\x90","\x90","\x90",
	"\x90","\x90","\x90","\x90"
};

bool AmmoStatus;
char AmmoOpCode[] = "\xFF\x06";
char AmmoDefaultOpCode[] = "\xFF\x0E";
DWORD AmmoAddress = 0x4637E9;

bool HealthStatus;
char HealthValue[] = "\x39\x5";
char HealthDefaultValue[] = "\x64\x0";
DWORD HealthBaseAddress = { 0x00509B74 };
DWORD HealthOffsets[] = { 0xF8 };
DWORD AddressToWrite; // ?????

bool RfireStatus;
char RfireOpCode[] = "\x90\x90";
char RfireDefaultOpCode[] = "\x89\x10";
DWORD RfireAddress = 0x4637E4;

bool AutoGunStatus;
char AutoGunOpCode[] = "\xEB\x09";
char AutoGunDefaultOpCode[] = "\x75\x09";
DWORD AutoGunAddress = 0x463716;

int OnePressTimer = clock();
int GameTimer = clock();

bool UpdateOnNextRun;

DWORD WINAPI Main_Thread(LPVOID lpParam)
{
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);

	while (!GetAsyncKeyState(VK_INSERT))
	{

		if (clock() - GameTimer > 100)
		{
			if (HealthStatus)
			{
				if(AddressToWrite != NULL)
					WriteToMemory(AddressToWrite, HealthValue, 2);
			}
		}

		if (clock() - OnePressTimer > 400)
		{
			if (GetAsyncKeyState(VK_F1)) // Ammo Decrease
			{
				OnePressTimer = clock();
				AmmoStatus = !AmmoStatus;
				UpdateOnNextRun = true;
				if(!AmmoStatus)
					WriteToMemory(AmmoAddress, AmmoDefaultOpCode, 2);
				else
					WriteToMemory(AmmoAddress, AmmoOpCode, 2);
			}
			else if (GetAsyncKeyState(VK_F2)) // Rapid Fire
			{
				OnePressTimer = clock();
				RfireStatus = !RfireStatus;
				UpdateOnNextRun = true;
				if (!RfireStatus)
					WriteToMemory(RfireAddress, RfireDefaultOpCode, 2);
				else
					WriteToMemory(RfireAddress, RfireOpCode, 2);
			}
			else if (GetAsyncKeyState(VK_F3)) // Automatic
			{
				OnePressTimer = clock();
				AutoGunStatus = !AutoGunStatus;
				UpdateOnNextRun = true;
				if (!AutoGunStatus)
					WriteToMemory(AutoGunAddress, AutoGunDefaultOpCode, 2);
				else
					WriteToMemory(AutoGunAddress, AutoGunOpCode, 2);
			}
			else if (GetAsyncKeyState(VK_F4)) // Health
			{
				OnePressTimer = clock();
				HealthStatus = !HealthStatus;
				AddressToWrite = FindDmaAddy(1, HealthOffsets, HealthBaseAddress);
				UpdateOnNextRun = true;
				
			}
		}
		if (UpdateOnNextRun)
		{
			system("cls");
			cout << "-------------------------------------------------------------" << endl;
			cout << "               Internal Assault Cube Hacker                  " << endl;
			cout << "-------------------------------------------------------------" << endl << endl;
			cout << "[F1] Unlimited Ammo : " << boolalpha << AmmoStatus << endl;
			cout << "[F2] Rapid Fire  : " << boolalpha << RfireStatus << endl;
			cout << "[F3] Automatic Guns : " << boolalpha << AutoGunStatus << endl;
			cout << "[F4] Unlimited Health : " << boolalpha << HealthStatus << endl;
			UpdateOnNextRun = false;
		}
	}
	return S_OK;
}

void WriteToMemory(DWORD addressToWrite, char* valueToWrite, int byteNum)
{
	unsigned long oldProtection;
	VirtualProtect((LPVOID)addressToWrite, byteNum, PAGE_EXECUTE_READWRITE, &oldProtection);
	memcpy((LPVOID)addressToWrite, valueToWrite, byteNum);
	VirtualProtect((LPVOID)addressToWrite, byteNum, oldProtection, NULL);
}

DWORD FindDmaAddy(int PointerLevel, DWORD Offsets[], DWORD BaseAddress)
{
	DWORD pointer = BaseAddress;
	if (pointer == 0)
		return NULL;

	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == PointerLevel - 1)
		{
			pointer = pointer + Offsets[i];
			return pointer;
		}
		else
		{
			pointer = pointer + Offsets[i];
			if (pointer == 0)
				return NULL;
		}
	}
	return pointer;
}

BOOL APIENTRY DllMain(HMODULE hModule,DWORD _reason,LPVOID lpReserved)
{
	if (_reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0x1000, &Main_Thread, 0, 0, NULL);
	}
	return true;
}
/*
ammo decrease
4637E9
DEF OPCODE FF0E
OPCODE = FF06




rapid fire
4637E4
DEF OPCODE 8910
OPCODE = NOP 90


automatic
463716
DEF OPCODE 75 09
OPCODE EB 09


*/