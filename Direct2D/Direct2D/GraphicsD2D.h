#pragma once
#include <d2d1.h>

class GraphicsD2D
{
public:
	GraphicsD2D(){}

	bool Initialize(HWND &hWnd);

	void Draw();
private:
	ID2D1Factory *pD2DFactory;
	ID2D1HwndRenderTarget *pD2DRenderTarget;

	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1SolidColorBrush* pYellowBrush;
	ID2D1SolidColorBrush* pWhiteBrush;
};
