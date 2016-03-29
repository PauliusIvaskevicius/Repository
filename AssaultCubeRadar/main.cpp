#include <windows.h> 
#include <tchar.h>    
#include <string>
#include <ctime>
#include "DirectX.h"
#include <math.h>

using namespace std;

typedef std::basic_string<TCHAR> ustring;

class Vec3D {
public:
	float x, y, z;
};
class Vec2D {
public:
	float x, y;
};


void FindWindowTool(HWND& hGameWindow, HANDLE& hProcHandle, DWORD& dwProcID, string& GameStatus, LPCSTR LGameWindow);
Vec2D RotatePoint(Vec2D pointToRotate, Vec2D centerPoint, float angle, bool angleInRadians);

float zoom = 1.0;

RECT m_Rect;
HDC HDC_Desktop;

HBRUSH PlayerBrush;
HBRUSH EnemyBrush;

LPDIRECT3DTEXTURE9 tex;
LPD3DXSPRITE sprite;
D3DXVECTOR3 position;
D3DCOLOR color;
bool spriteInit = true;
Sprait gCom;

Vec2D centerPoint;
string GameName = "AssaultCube";
LPCSTR LGameWindow = "AssaultCube";
string GameStatus = "OK";

HWND hGameWindow = NULL;
HANDLE hProcHandle = NULL;
DWORD dwProcID = NULL;

UINT_PTR localPlayerAddr = 0;
UINT_PTR playerArrayPointer = 0x50F4F8;
UINT_PTR playerArrayAddress = 0;

int NumOfPlayers = 8;

int i_timer = clock();

struct MyPlayer
{
	UINT_PTR CLocalPlayer;
	int Health;
	float angleY;
	int SMGAmmo;
	char Name[16];
	BYTE Team;
	Vec3D Position;

	void ReadInformation()
	{
		CLocalPlayer = localPlayerAddr;
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0xF8), &Health, sizeof(int), 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0x225), &Name, 16, 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0x150), &SMGAmmo, sizeof(int), 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0x32c), &Team, 1, 0);
		ReadProcessMemory(hProcHandle, (PBYTE*)(localPlayerAddr + 0x40), &angleY, sizeof(float), 0);
		ReadProcessMemory(hProcHandle, (LPCVOID)(localPlayerAddr + 0x4), &Position, sizeof(Position), NULL);
		ReadProcessMemory(hProcHandle, (LPCVOID)0x510D98, &NumOfPlayers, sizeof(NumOfPlayers), NULL);

	}
}MyPlayer;
struct PlayerList
{

	DWORD dw_Loop_Every = 0x4;
	UINT_PTR CBaseEntity;
	Vec3D Position;
	int Health;
	BYTE Team;

	void ReadInformation(int Player)
	{
		ReadProcessMemory(hProcHandle, (PBYTE*)(playerArrayAddress + dw_Loop_Every + (Player * dw_Loop_Every)), &CBaseEntity, sizeof(DWORD), 0);

		ReadProcessMemory(hProcHandle, (PBYTE*)(CBaseEntity + 0x4), &Position, sizeof(float[3]), 0);

		ReadProcessMemory(hProcHandle, (PBYTE*)(CBaseEntity + 0xF8), &Health, sizeof(int), 0);

		ReadProcessMemory(hProcHandle, (PBYTE*)(CBaseEntity + 0x32c), &Team, 1, 0);
	}
}PlayerList[32];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
inline int ErrMsg(const ustring&);
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR pStr, int nCmd)
{
	ustring classname = _T("AssaultCube Radar By NobodyLTU(TheKirvis)");
	WNDCLASSEX wcx = { 0 };  //used for storing information about the wnd 'class'

	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.lpfnWndProc = WndProc;            
	wcx.hInstance = hInst;
	wcx.hIcon = reinterpret_cast<HICON>(LoadImage(0, IDI_APPLICATION,
		IMAGE_ICON, 0, 0, LR_SHARED));
	wcx.hCursor = reinterpret_cast<HCURSOR>(LoadImage(0, IDC_ARROW,
		IMAGE_CURSOR, 0, 0, LR_SHARED));
	wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_BTNFACE + 1);
	wcx.lpszClassName = classname.c_str();
	//the window 'class' (not c++ class) has to be registered with the system
	//before windows of that 'class' can be created
	if (!RegisterClassEx(&wcx))
	{
		ErrMsg(_T("Failed to register window class"));
		return -1;
	}

	int desktopwidth = GetSystemMetrics(SM_CXSCREEN);
	int desktopheight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd = CreateWindowEx( 0,                     //extended styles
		classname.c_str(),     //name: wnd 'class'
		_T("AssaultCube Radar By NobodyLTU(TheKirvis)"),   //wnd title
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,   //wnd style
		desktopwidth / 4,        //position:left
		desktopheight / 4,       //position: top
		500,        //width
		500,       //height
		0,                     //parent wnd handle
		0,                     //menu handle/wnd id
		hInst,                 //app instance
		0);                    //user defined info
	if (!hwnd)
	{
		ErrMsg(_T("Failed to create wnd"));
		return -1;
	}

	ShowWindow(hwnd, nCmd);
	UpdateWindow(hwnd);
	//------------------------------------------------------------------------------------RADAR
	FindWindowTool(hGameWindow, hProcHandle, dwProcID, GameStatus, LGameWindow);
	ReadProcessMemory(hProcHandle, (LPCVOID)0x509B74, &localPlayerAddr, 4, NULL);
	ReadProcessMemory(hProcHandle, (LPCVOID)playerArrayPointer, &playerArrayAddress, 4, NULL);



	GetWindowRect(hwnd, &m_Rect);

	centerPoint.x = m_Rect.right - m_Rect.left;
	centerPoint.y = m_Rect.bottom - m_Rect.top;


	


	if (!gDX.D3DInit(hwnd))
		return 0;

	gCom.Init(0, -20, "radar.png");


	MSG msg;
	while (GetMessage(&msg, 0, 0, 0)>0)
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return static_cast<int>(msg.wParam);

}
//=============================================================================
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	switch (uMsg)
	{

	case WM_PAINT:
		if (gDX.dx_Device != NULL)
		{
			
			if (spriteInit)
			{
				
				gDX.dx_Device->Clear(NULL, NULL, D3DCLEAR_TARGET, D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, NULL);
				gDX.dx_Device->BeginScene();
	

				gCom.Draw();
				gDX.FillRGBA((centerPoint.x / 2)  - ((2 * zoom) / 2), (centerPoint.y / 2) - 8, 4 * zoom, 8 * zoom, 0, 255, 255, 255);
				//gDX.FillRGBA((centerPoint.x / 2), (centerPoint.y / 2), 3 * zoom, 3 * zoom, 255, 255, 255, 255);
				if (GetAsyncKeyState(VK_OEM_PLUS))
					zoom += 0.1;
				if (GetAsyncKeyState(VK_OEM_MINUS))
				{
					if (zoom - 0.1 <= 0)
						zoom = 0.1;
					zoom -= 0.1;

				}

				for (int i = 0; i < NumOfPlayers; i++)
				{
					MyPlayer.ReadInformation();
					PlayerList[i].ReadInformation(i);

					if (PlayerList[i].Health <= 0)
						continue;

					Vec2D PlayerListPos;
					PlayerListPos.x = PlayerList[i].Position.x * zoom;
					PlayerListPos.y = PlayerList[i].Position.y * zoom;

					Vec2D MyPlayer2D;
					MyPlayer2D.x = MyPlayer.Position.x * zoom;
					MyPlayer2D.y = MyPlayer.Position.y * zoom;

					Vec2D PositionToDraw = RotatePoint(PlayerListPos, MyPlayer2D, MyPlayer.angleY, false);

					if (MyPlayer.Team == PlayerList[i].Team)
					{
						gDX.FillRGBA(
							PositionToDraw.x + (250.0 - MyPlayer2D.x),
							PositionToDraw.y + (250.0 - MyPlayer2D.y),
							5 * zoom, 5 * zoom, 255,127,80, 0);
					}
					else
					{
						gDX.FillRGBA(
							PositionToDraw.x + (250.0 - MyPlayer2D.x),
							PositionToDraw.y + (250.0 - MyPlayer2D.y),
							5 * zoom, 5 * zoom, 255, 0, 0, 0);
					}



				}
				
				gDX.dx_Device->EndScene();
				gDX.dx_Device->PresentEx(NULL, NULL, NULL, NULL, NULL);

			}
			
		}
		

		break;

	case WM_DESTROY:
		PostQuitMessage(0);    //signal end of application
		return 0;
	default:
		//let system deal with msg
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
inline int ErrMsg(const ustring& s)
{
	return MessageBox(0, s.c_str(), _T("ERROR"), MB_OK | MB_ICONEXCLAMATION);
}


void FindWindowTool(HWND& hGameWindow, HANDLE& hProcHandle, DWORD& dwProcID, string& GameStatus, LPCSTR LGameWindow)
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
Vec2D RotatePoint(Vec2D pointToRotate, Vec2D centerPoint, float angle, bool angleInRadians)
{
	if (!angleInRadians)
		angle = (float)(angle * (3.14 / -180.0));
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);
	Vec2D returnVec;
	returnVec.x = cosTheta * (pointToRotate.x - centerPoint.x) - sinTheta * (pointToRotate.y - centerPoint.y);
	returnVec.y = sinTheta * (pointToRotate.x - centerPoint.x) + cosTheta * (pointToRotate.y - centerPoint.y);

	returnVec.x += centerPoint.x;
	returnVec.y += centerPoint.y;
	return returnVec;
}