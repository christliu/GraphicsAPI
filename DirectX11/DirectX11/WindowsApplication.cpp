#include "WindowsApplication.h"
#include <iostream>

WindowsApplication::WindowsApplication():m_appTitle("WinApp"), m_screenWidth(800), m_screenHeight(600), m_appName("ChristJunior")
{
}

bool WindowsApplication::Initialize()
{
	std::cout << "WindowsApp Init" << std::endl;
	HINSTANCE hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = m_appName;

	RegisterClassEx(&wc);

	m_hwnd = CreateWindowEx(
		0,
		m_appName,
		m_appTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		m_screenWidth,
		m_screenHeight,
		NULL,
		NULL,
		hInstance,
		this
	);

	ShowWindow(m_hwnd, SW_SHOW);

	return true;
}

void WindowsApplication::Tick()
{
	MSG msg;
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowsApplication::Finalize()
{

}

LRESULT CALLBACK WindowsApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowsApplication *pThis;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WindowsApplication*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WindowsApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	switch (message)
	{

	case WM_PAINT:
		g_pApp->OnDraw();
		break;

	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		g_pApp->OnMouseMove(x, y);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		g_pApp->Quit();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}