#pragma once
#include <d2d1.h>

class GraphicsD2D
{
public:
	GraphicsD2D():pD2DFactory(NULL), pD2DRenderTarget(NULL), pBlackBrush(NULL), pYellowBrush(NULL), pWhiteBrush(NULL), mouseX(0), mouseY(0){}

	bool Initialize(HWND &hWnd);

	void Draw();

	void OnMouseMove(int x, int y);
private:
	ID2D1Factory *pD2DFactory;
	ID2D1HwndRenderTarget *pD2DRenderTarget;

	ID2D1SolidColorBrush* pBlackBrush;
	ID2D1SolidColorBrush* pYellowBrush;
	ID2D1SolidColorBrush* pWhiteBrush;

	int mouseX, mouseY;
};
