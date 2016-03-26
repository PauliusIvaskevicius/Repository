#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>


#include <Dwmapi.h> 
#pragma comment(lib, "dwmapi.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")


class dx
{
public:
	IDirect3D9Ex* dx_Object;
	IDirect3DDevice9Ex* dx_Device;
	D3DPRESENT_PARAMETERS dx_Params;

	bool D3DInit(HWND hwnd);
	void D3DShutdown();
	void FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a);
	void BoxFillRGBA(int x, int y, int x2, int y2, int r, int g, int b, int a);
};
extern dx gDX;