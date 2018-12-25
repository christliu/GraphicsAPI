#pragma once
#include <d3d11.h>

#include "DirectXMath.h"
#include "DirectXPackedVector.h"
#include "DirectXColors.h"


class DX11Graphics
{
public:
	DX11Graphics(HWND& hwnd):m_hwnd(hwnd), mEnable4xMsaa(FALSE) {};

	bool Initialize();
	void OnDraw();

	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};
private:

	void CreateRenderTarget();
	void SetViewport();

	void BuildGeometryBuffers();
	void BuildFx();
	void BuildVertexLayout();
	void UpdateScene();

//	void InitPipeline();

	SIZE WindowSize();
	float AspectRatio() { SIZE sz = WindowSize(); return float(sz.cx) / float(sz.cy); };

	IDXGISwapChain *m_pSwapChain;
	D3D_FEATURE_LEVEL *m_pFeatureLevel;
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pImmediateContext;
	ID3D11RenderTargetView *m_pRtv;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;
	HWND m_hwnd;


	ID3D11InputLayout *m_pInputLayout;
	ID3D11Buffer *m_pVB;
	ID3D11Buffer *m_pIB;

	ID3D11VertexShader *m_pVertexShader;	// 顶点着色器
	ID3D11PixelShader *m_pPixelShader;		// 像素着色器
	ID3DBlob *m_pVertexBlob;
	ID3D11Buffer *m_pCB;
	ConstantBuffer mCBuffer;	// 用于修改GPU常量缓冲区的变量

	bool mEnable4xMsaa;
	UINT m4xMsaaQuality;


	D3D_FEATURE_LEVEL m_featureLevel;
};