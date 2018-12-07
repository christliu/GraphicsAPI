#include "stdafx.h"
#include "D2DApp.h"

bool D2DApp::Initialize(HWND& hWnd)
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	if (FAILED(hr))
	{
		return FALSE;
	}
	RECT rc;
	GetClientRect(hWnd, &rc);
	hr = pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(
		hWnd, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)), &pD2DRenderTarget);
	if (FAILED(hr))
	{
		return FALSE;
	}
	hr = pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &pBlackBrush);
	if (FAILED(hr))
	{
		return FALSE;
	}
	return TRUE;
}

void D2DApp::Finalize()
{

}

void D2DApp::OnDraw()
{
	pD2DRenderTarget->BeginDraw();
	pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

	D2D1_SIZE_F rtSize = pD2DRenderTarget->GetSize();
	pD2DRenderTarget->DrawRectangle(D2D1::RectF(0 + 10.0f, 0 + 10.0f, rtSize.width - 10.0f, rtSize.height - 10.0f), pBlackBrush);
	pD2DRenderTarget->EndDraw();
}