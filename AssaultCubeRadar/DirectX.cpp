#pragma once

#include <Windows.h>
#include <tlhelp32.h>
#include <iostream>


#include <Dwmapi.h> 
#pragma comment(lib, "dwmapi.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "DirectX.h"



dx gDX;

bool dx::D3DInit(HWND hwnd)
{
	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &dx_Object)))
		return false;

	dx_Params.BackBufferFormat = D3DFMT_A8R8G8B8;
	dx_Params.hDeviceWindow = hwnd;
	dx_Params.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	dx_Params.SwapEffect = D3DSWAPEFFECT_DISCARD;
	dx_Params.Windowed = true;

	if (FAILED(dx_Object->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &dx_Params, NULL, &dx_Device)))
		return false;

	return true;
}

void dx::D3DShutdown()
{
	if (dx_Device != NULL)
		dx_Device->Release();

	if (dx_Object != NULL)
		dx_Object->Release();
}

void dx::FillRGBA(int x, int y, int w, int h, int r, int g, int b, int a)
{
	D3DCOLOR col = D3DCOLOR_RGBA(r, g, b, a);
	D3DRECT rec = { x, y, x + w, y + h };
	dx_Device->Clear(1, &rec, D3DCLEAR_TARGET, col, 0, 0);
}

void dx::BoxFillRGBA(int x, int y, int x2, int y2, int r, int g, int b, int a)
{
	FillRGBA(x - 1, y - 1, x2 + 2, 1, r, g, b, a);
	FillRGBA(x - 1, y, 1, y2 - 1, r, g, b, a);
	FillRGBA(x + x2, y, 1, y2 - 1, r, g, b, a);
	FillRGBA(x - 1, y + y2 - 1, x2 + 2, 1, r, g, b, a);
}