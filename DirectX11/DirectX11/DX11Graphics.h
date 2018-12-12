#pragma once
#include <d3d11.h>

class DX11Graphics
{
public:
	DX11Graphics() {};

	bool Initialize(HWND& hWnd);

private:
	void CreateRenderTarget();
	void SetViewport();
	void InitPipeline();
	void InitGraphics();

	IDXGISwapChain *m_pSwapChain;
	D3D_FEATURE_LEVEL *m_pFeatureLevel;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmediateContext;
	ID3D11RenderTargetView *m_pRtv;

	D3D_FEATURE_LEVEL m_featureLevel;
};