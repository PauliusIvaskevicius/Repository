#include <iostream>
#include <Windows.h>

using namespace std;


void RightClick();
void LeftClick();
void MouseMove(int x, int y, RECT rect);

int main()
{
	RECT rect;
	HWND hwnd;

	hwnd = FindWindow(NULL, TEXT("Task Manager"));
	if (hwnd == NULL)
	{
		cout << "Calculator not found" << endl;
	}
	else
	{
		cout << "Calculator found" << endl;
	}
	

	
	while (true)
	{
		GetWindowRect(hwnd, &rect);
		cout << rect.left << "LEFT" << endl;
		cout << rect.right << "RIGHT" << endl;
		cout << rect.top << "UP" << endl;
		cout << rect.bottom << "BOTTOM" << endl << endl << endl;
		Sleep(2000);
		MouseMove(0, 0, rect);
	}
	
}
void LeftClick()
{
	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// left up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void RightClick()
{
	INPUT    Input = { 0 };
	// right down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	::SendInput(1, &Input, sizeof(INPUT));

	// right up
	::ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	::SendInput(1, &Input, sizeof(INPUT));
}

void MouseMove(int x, int y, RECT rect)
{
	/*
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN) - 1;
	cout << fScreenWidth << endl;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN) - 1;
	cout << fScreenHeight << endl;
	*/
	double fScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	cout << fScreenWidth << endl;
	double fScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	cout << fScreenHeight << endl;
	double fx = (x + rect.left) *(65535.0f / fScreenWidth) ;
	double fy = (y + rect.top ) *(65535.0f / fScreenHeight) ;
	INPUT  Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
	Input.mi.dx = fx;
	Input.mi.dy = fy;
	::SendInput(1, &Input, sizeof(INPUT));
}