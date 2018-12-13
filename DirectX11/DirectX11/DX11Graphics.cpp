#include "DX11Graphics.h"
#include <iostream>
using namespace std;


SIZE DX11Graphics::WindowSize()
{
	RECT rt;
	GetWindowRect(m_hwnd, &rt);
	/*
	SIZE size;
	size.cx = rt.left - rt.right;
	size.cy = rt.bottom - rt.top;
	*/
	return SIZE{ rt.right - rt.left, rt.bottom - rt.top };
}

bool DX11Graphics::Initialize()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	SIZE windowSize = WindowSize();
	sd.BufferCount = 1;
	sd.BufferDesc.Width = int(windowSize.cx);
	sd.BufferDesc.Height = int(windowSize.cy);
	//sd.BufferDesc.Width = 500;
	//sd.BufferDesc.Height = 500;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = m_hwnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	D3D_FEATURE_LEVEL FeatureLevels = D3D_FEATURE_LEVEL_10_1;

	HRESULT hr = S_OK;
	if (FAILED(hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_WARP,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_pSwapChain,
		&m_pDevice,
		&m_featureLevel,
		&m_pImmediateContext)))
	{
		cout << "D3D11CreateDeviceAndSwapChain Failed " << hr << endl;
		return FALSE;
	}
	cout << "D3D11CreateDeviceAndSwapChain Success with FeatureLevel " << m_featureLevel << endl;

	CreateRenderTarget();
	cout << "CreateRenderTargetView Success" << endl;
	return TRUE;
}

void DX11Graphics::CreateRenderTarget()
{
	HRESULT hr;
	ID3D11Texture2D *pBackBuffer;

	// Get a pointer to the back buffer
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D),
		(LPVOID*)&pBackBuffer);

	// Create a render-target view
	m_pDevice->CreateRenderTargetView(pBackBuffer, NULL,
		&m_pRtv);
	pBackBuffer->Release();

	// Bind the view
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRtv, NULL);
}

void DX11Graphics::SetViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	SIZE sz = WindowSize();

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = sz.cx;
	viewport.Height = sz.cy;

	m_pImmediateContext->RSSetViewports(1, &viewport);
}