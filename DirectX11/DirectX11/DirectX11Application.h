#pragma once
#include "WindowsApplication.h"
#include <d3d11.h>
#include <DirectXMath.h>

class DirectX11Application : public WindowsApplication
{
public:
	bool Initialize();

	void OnDraw();

protected:
	struct ConstantBuffer
	{
		DirectX::XMMATRIX world;
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX proj;
	};

	bool DX11Initialize();

	// Pipeline Init
	bool DX11PipelineInitialize();
	bool CreateDeviceAndSwapchain();
	bool CreateRenderTarget();
	void SetViewport();

	// Graphics Related Initialize
	void GraphicsInitialize();
	void BuildGeometryBuffers();
	void BuildConstantBuffer();
	void BuildFX();
	void BuildVertexLayout();

	float AspectRatio() { return float(m_screenWidth) / float(m_screenHeight); };


protected:
	// DX11Pipeline

	//SwapChain and Device
	IDXGISwapChain *m_pSwapChain;
	ID3D11Device *m_pDevice;
	D3D_FEATURE_LEVEL m_featureLevel;
	ID3D11DeviceContext *m_pImmediateContext;
	bool mEnable4xMsaa;
	UINT m4xMsaaQuality;

	// RTV
	ID3D11RenderTargetView *m_pRtv;
	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	// Graphics Buffer
	ID3D11Buffer *m_pVB;
	ID3D11Buffer *m_pIB;
	ID3D11InputLayout *m_pInputLayout;

	//ConstantBuffer
	ConstantBuffer mCBuffer;
	ID3D11Buffer *m_pCB;

	// Shader
	ID3D11VertexShader *m_pVertexShader;	// 顶点着色器
	ID3D11PixelShader *m_pPixelShader;		// 像素着色器
	ID3DBlob *m_pVertexBlob;
};
