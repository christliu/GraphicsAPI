#include "WindowBaseApplication.h"

const char* WindowBaseApplication::m_appName = "WindowsApp";
const char* WindowBaseApplication::m_winTitle = "WindowsApp";
const int WindowBaseApplication::m_screenWidth = 800;
const int WindowBaseApplication::m_screenHeight = 600;

bool WindowBaseApplication::Initialize()
{
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

	m_hWnd = CreateWindowEx(
		0,
		m_appName,
		m_winTitle,
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

	ShowWindow(m_hWnd, SW_SHOW);

	return m_graphics.Initialize(m_hWnd);
}

void WindowBaseApplication::Tick()
{
	MSG msg;
	if (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void WindowBaseApplication::Quit()
{
	m_bQuit = true;
}

void WindowBaseApplication::Finalize()
{

}

void WindowBaseApplication::OnDraw()
{
	m_graphics.Draw();
}

LRESULT CALLBACK WindowBaseApplication::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WindowBaseApplication *pThis;

	if (message == WM_NCCREATE)
	{
		pThis = static_cast<WindowBaseApplication*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);

		SetLastError(0);
		if (!SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis)))
		{
			if (GetLastError() != 0)
				return FALSE;
		}
	}
	else
	{
		pThis = reinterpret_cast<WindowBaseApplication*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	switch (message)
	{
	case WM_PAINT:
		pThis->OnDraw();
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		pThis->Quit();
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;

}