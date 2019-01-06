#pragma once
#include <windowsx.h>
#include <Windows.h>
#include "BaseApplication.h"

class WindowsApplication: public BaseApplication
{
public:
	WindowsApplication();

protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool Initialize();
	void Tick();
	void Finalize();

protected:
	HWND m_hwnd;

	const char* m_appTitle;
	const char* m_appName;

	size_t m_screenWidth, m_screenHeight;
};