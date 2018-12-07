#pragma once
#include <windowsx.h>
#include <Windows.h>

class WindowBaseApplication
{
public:
	WindowBaseApplication() :m_bQuit(false) {};

	bool Initialize();
	void Tick();
	void Finalize();
	
	bool IsQuit() { return m_bQuit; }

	void Quit();
	void OnDraw();

	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:


	bool m_bQuit;
	HWND m_hWnd;

	static const char* m_appName;
	static const char* m_winTitle;
	static const int m_screenWidth;
	static const int m_screenHeight;
};
