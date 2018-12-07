#pragma once
#include <d2d1.h>

class D2DApp
{
public:
	D2DApp() : pD2DFactory(NULL), pD2DRenderTarget(NULL), pBlackBrush(NULL) {};

	bool Initialize(HWND &hWnd);

	void Finalize();

	void OnDraw();

private:
	ID2D1Factory * pD2DFactory;
	ID2D1HwndRenderTarget* pD2DRenderTarget;
	ID2D1SolidColorBrush* pBlackBrush;
};