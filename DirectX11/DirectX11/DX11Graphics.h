#pragma once
#include <d3d11.h>

class DX11Graphics
{
public:
	DX11Graphics(HWND& hwnd):m_hwnd(hwnd) {};

	bool Initialize();
	void OnDraw();
private:

	void CreateRenderTarget();
	void SetViewport();
	void InitPipeline();
	void InitGraphics();

	SIZE WindowSize();

	IDXGISwapChain *m_pSwapChain;
	D3D_FEATURE_LEVEL *m_pFeatureLevel;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmediateContext;
	ID3D11RenderTargetView *m_pRtv;
	HWND m_hwnd;

	ID3D11VertexShader *m_pVS;
	ID3D11PixelShader *m_pPS;
	ID3D11InputLayout *m_pInputLayout;
	ID3D11Buffer *m_pBuffer;

	D3D_FEATURE_LEVEL m_featureLevel;
};