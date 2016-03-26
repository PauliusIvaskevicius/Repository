#include <Windows.h>
#include <sstream>
#include <iostream>
#include <math.h> 
#include "Functions.h"
#include <vector>
#include <algorithm> 
#include <vector>
using namespace std;

class Vec3D {
public:
	float x, y, z;
};

float Get3dDistance(Vec3D myCoords, Vec3D enemyCoords);
void DrawBorderBox(int x, int y, int w, int h, int thickness);
void DrawFilledRect(int x, int y, int w, int h);
void DrawLine(float StartX, float StartY, float EndX, float EndY, COLORREF Pen);
void DrawString(int x, int y, COLORREF color, const char* text);
bool WorldToScreen(Vec3D In, Vec3D& Out, float * ViewMatrix);

RECT m_Rect;
HDC HDC_Desktop;

HWND hGameWindow = NULL;
HANDLE hProcHandle = NULL;
DWORD dwProcID = NULL;

string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
string GameStatus = "OK";

const DWORD dw_vMatrix = 0x501AE8;
HFONT Font;
HBRUSH EnemyBrush;
// Colors
COLORREF SnapLineCOLOR;
COLORREF TextCOLOR;
// ------------------------
typedef struct
{
	float flMatrix[4][4];
}WorldToScreenMatrix_t;

// Players

UINT_PTR localPlayerAddr = 0;
UINT_PTR playerArrayPointer = 0x50F4F8;//multiplayer
UINT_PTR playerArrayAddress = 0;


const DWORD Player_Count = 0x55D6F08;

const DWORD dw_Player_HP = 0xF8;
const DWORD dw_Player_ARM = 0xFC;
const DWORD dw_Player_SMGAMMO = 0x150;
const DWORD dw_Player_PISTAMMO = 0x44;
const DWORD dw_Player_NAME = 0x12D;


const DWORD dw_Loop_Every = 0x4;
const DWORD dw_Player_Other_NAME = 0x225;

int NumOfPlayers = 8;



int i_Timer = clock();

struct MyPlayer
{
	UINT_PTR CLocalPlayer;
	int Health = 1;
	int SMGAmmo = 100;
	float viewMatrix[16];
	Vec3D Position;

	void ReadInformation()
	{
		CLocalPlayer = localPlayerAddr;
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr+ dw_Player_HP), &Health, sizeof(int), 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + dw_Player_SMGAMMO), &SMGAmmo, sizeof(int), 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0x4), &Position, sizeof(float[3]), 0);
		ReadProcessMemory(hProcHandle, (LPCVOID)dw_vMatrix, &viewMatrix, sizeof(viewMatrix), NULL);
	}
	//00501AE8 View Matrix dw_vMatrix
}MyPlayer;

struct PlayerList
{
	UINT_PTR CBaseEntity;
	Vec3D Position;
	int Health;
	void ReadInformation(int Player)
	{
		ReadProcessMemory(hProcHandle, (PBYTE*)(playerArrayAddress + dw_Loop_Every + (Player * dw_Loop_Every)) , &CBaseEntity, sizeof(DWORD), 0);
		
		ReadProcessMemory(hProcHandle, (PBYTE*)(CBaseEntity + 0x4), &Position, sizeof(float[3]), 0);

		ReadProcessMemory(hProcHandle, (PBYTE*)(CBaseEntity + 0xF8), &Health, sizeof(int), 0);
	}
}PlayerList[32];



void SetupDrawing(HDC hDesktop, HWND handle)
{
	HDC_Desktop = hDesktop;
	HANDLE handl = handle;
	EnemyBrush = CreateSolidBrush(RGB(255, 0, 0));
	//Color
	SnapLineCOLOR = RGB(0, 0, 255);
	TextCOLOR = RGB(0, 255, 0);
}

void DrawESP(int x, int y, float distance)
{
	//ESP RECTANGLE
	int width = 18100 / distance;
	int height = 37500 / distance;
	DrawBorderBox(x - (width / 2), y - height, width, height, 1);

	//Sandwich ++
	DrawLine((m_Rect.right - m_Rect.left) / 2,
		m_Rect.bottom - m_Rect.top, x, y,
		SnapLineCOLOR);


	std::stringstream ss;
	ss << (int)distance;

	char * distanceInfo = new char[ss.str().size() + 1];
	strcpy(distanceInfo, ss.str().c_str());

	DrawString(x, y, TextCOLOR, distanceInfo);

	delete[] distanceInfo;
}

void ESP()
{
	GetWindowRect(hGameWindow, &m_Rect);
	for (int i = 0; i < 8; i++)
	{
		PlayerList[i].ReadInformation(i);


		Vec3D EnemyXY;
		EnemyXY.x = PlayerList[i].Position.x;
		EnemyXY.y = PlayerList[i].Position.y;
		if (WorldToScreen(PlayerList[i].Position, EnemyXY,MyPlayer.viewMatrix))
		{
			DrawESP(EnemyXY.x - m_Rect.left, EnemyXY.y - m_Rect.top, Get3dDistance(MyPlayer.Position, PlayerList[i].Position));
		}

	}
}

DWORD Offsets[] = { 0x0,0x0 };

int main()
{
	

	cout << " PlayerBase " << playerArrayAddress;
	FindWindowTool(hGameWindow, hProcHandle, dwProcID, GameStatus, LGameWindow);
	HDC_Desktop = GetDC(hGameWindow);
	SetupDrawing(HDC_Desktop, hGameWindow);

	ReadProcessMemory(hProcHandle, (LPCVOID)0x509B74, &localPlayerAddr, 4, NULL);
	ReadProcessMemory(hProcHandle, (LPCVOID)playerArrayPointer, &playerArrayAddress, 4, NULL);
	while(true)
		ESP();

	/*while (!GetAsyncKeyState(VK_INSERT))
	{
		system("cls");
		MyPlayer.ReadInformation();
		ESP();
		cout << MyPlayer.Position.x << " " << MyPlayer.Position.y << " " << MyPlayer.Position.z <<  endl;
		cout << playerArrayAddress << endl;
		for (int i = 0; i < NumOfPlayers; i++)
		{
			cout << "BOT " << i << " x , y , z : " << PlayerList[i].Position.x << " , " << PlayerList[i].Position.y << " " << PlayerList[i].Position.z << endl;
		}
		/*if (clock() - i_Timer > 100)
		{
			system("cls");
			cout << GameStatus << endl;
			//ReadProcessMemory(hProcHandle,  (LPCVOID)FindDmaAddy(0,hProcHandle,NULL,Player_Count), &NumOfPlayers, sizeof(int), 0);
			cout << NumOfPlayers << endl;

			
			for (int i = 0; i < NumOfPlayers; i++)
			{
				PlayerList[i].ReadInformation(i);
				cout << "BOT " <<  i << " x , y , z : " << PlayerList[i].Position[0] << " , " << PlayerList[i].Position[1]<< PlayerList[i].Position[2] << endl;
			}
			
			cout << "Player HP : " << MyPlayer.Health << endl;
			//cout << "Player Name : " << MyPlayer.Name << endl;
		}
	}
	*/
	return 0;
}


bool WorldToScreen(Vec3D In, Vec3D& Out, float * ViewMatrix) {
	Out.x = In.x * ViewMatrix[0] + In.y * ViewMatrix[4] + In.z * ViewMatrix[8] + ViewMatrix[12];
	Out.y = In.x * ViewMatrix[1] + In.y * ViewMatrix[5] + In.z * ViewMatrix[9] + ViewMatrix[13];
	Out.z = In.x * ViewMatrix[2] + In.y * ViewMatrix[6] + In.z * ViewMatrix[10] + ViewMatrix[14];
	float w = In.x * ViewMatrix[3] + In.y * ViewMatrix[7] + In.z * ViewMatrix[11] + ViewMatrix[15];

	if (w < 0.0)
		return false;

	Out.x /= w;
	Out.y /= w;
	Out.z /= w;

	Out.x *= 800 / 2.0f;
	Out.x += 800 / 2.0f;

	Out.y *= -600 / 2.0f;
	Out.y += 600 / 2.0f;

	return true;
}


void DrawFilledRect(int x, int y, int w, int h)
{
	//We create our rectangle to draw on screen
	RECT rect = { x, y, x + w, y + h };
	//We clear that portion of the screen and display our rectangle
	FillRect(HDC_Desktop, &rect, EnemyBrush);
}


void DrawBorderBox(int x, int y, int w, int h, int thickness)
{
	//Top horiz line
	DrawFilledRect(x, y, w, thickness);
	//Left vertical line
	DrawFilledRect(x, y, thickness, h);
	//right vertical line
	DrawFilledRect((x + w), y, thickness, h);
	//bottom horiz line
	DrawFilledRect(x, y + h, w + thickness, thickness);
}


//Here is where we draw our line from point A to Point B
void DrawLine(float StartX, float StartY, float EndX, float EndY, COLORREF Pen)
{
	int a, b = 0;
	HPEN hOPen;
	// penstyle, width, color
	HPEN hNPen = CreatePen(PS_SOLID, 2, Pen);
	hOPen = (HPEN)SelectObject(HDC_Desktop, hNPen);
	// starting point of line
	MoveToEx(HDC_Desktop, StartX, StartY, NULL);
	// ending point of line
	a = LineTo(HDC_Desktop, EndX, EndY);
	DeleteObject(SelectObject(HDC_Desktop, hOPen));
}

//Draw our text with this function
void DrawString(int x, int y, COLORREF color, const char* text)
{
	SetTextAlign(HDC_Desktop, TA_CENTER | TA_NOUPDATECP);

	SetBkColor(HDC_Desktop, RGB(0, 0, 0));
	SetBkMode(HDC_Desktop, TRANSPARENT);

	SetTextColor(HDC_Desktop, color);

	SelectObject(HDC_Desktop, Font);

	TextOutA(HDC_Desktop, x, y, text, strlen(text));

	DeleteObject(Font);
}
float Get3dDistance(Vec3D myCoords, Vec3D enemyCoords)
{
	return sqrt(
		pow(double(enemyCoords.x - myCoords.x), 2.0) +
		pow(double(enemyCoords.y - myCoords.y), 2.0) +
		pow(double(enemyCoords.z - myCoords.z), 2.0));

}