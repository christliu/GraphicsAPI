#pragma once
#include <windowsx.h>
#include <Windows.h>
#include "DX11Graphics.h"


class WindowBaseApplication
{
public:
	WindowBaseApplication() :m_bQuit(false), p_graphics(NULL){};

	bool Initialize();
	void Tick();
	void Finalize();
	
	bool IsQuit() { return m_bQuit; }

	void Quit();
	void OnDraw();
	void OnMouseMove(int x, int y);

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:


	bool m_bQuit;
	HWND m_hWnd;
	DX11Graphics* p_graphics;

	static const char* m_appName;
	static const char* m_winTitle;
	static const int m_screenWidth;
	static const int m_screenHeight;
};
