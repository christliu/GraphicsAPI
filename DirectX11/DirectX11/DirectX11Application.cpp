#include "DirectX11Application.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include "DirectXPackedVector.h"
#include "DirectXColors.h"
#include <iostream>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib,"d3dcompiler.lib")

struct VERTEX {
	XMFLOAT3    Position;
	XMFLOAT4    Color;
};


HRESULT CompileShader(_In_ LPCWSTR srcFile, _In_ LPCSTR entryPoint, _In_ LPCSTR profile, _Outptr_ ID3DBlob** blob)
{
	if (!srcFile || !entryPoint || !profile || !blob)
		return E_INVALIDARG;

	*blob = nullptr;

	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;
#endif

	const D3D_SHADER_MACRO defines[] =
	{
		"EXAMPLE_DEFINE", "1",
		NULL, NULL
	};

	ID3DBlob* shaderBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3DCompileFromFile(srcFile, defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		entryPoint, profile,
		flags, 0, &shaderBlob, &errorBlob);
	if (FAILED(hr))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}

		if (shaderBlob)
			shaderBlob->Release();

		return hr;
	}

	*blob = shaderBlob;

	return hr;
}

// DX11Pipeline
bool DirectX11Application::Initialize()
{
	if (!WindowsApplication::Initialize())
		return false;

	std::cout << "DirectX11 Init" << std::endl;

	return DX11Initialize();
}

void DirectX11Application::OnDraw()
{
	m_pImmediateContext->UpdateSubresource(m_pCB, 0, nullptr, &mCBuffer, 0, 0);
	// clear the back buffer to a deep blue
	const FLOAT clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRtv, Colors::Black);
	m_pImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_pImmediateContext->DrawIndexed(36, 0, 0);

	// swap the back buffer and the front buffer
	m_pSwapChain->Present(0, 0);
}


bool DirectX11Application::DX11Initialize()
{
	if (!DX11PipelineInitialize())
		return false;

	GraphicsInitialize();

	return true;
}

bool DirectX11Application::DX11PipelineInitialize()
{
	if (!CreateDeviceAndSwapchain())
		return false;

	if (!CreateRenderTarget())
		return false;
	cout << "CreateRenderTargetView Success" << endl;
	SetViewport();
	cout << "SetViewport Success" << endl;
	return true;
}

void DirectX11Application::GraphicsInitialize()
{
	BuildGeometryBuffers();
	BuildConstantBuffer();
	BuildFX();
	BuildVertexLayout();
}

bool DirectX11Application::CreateDeviceAndSwapchain()
{
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferCount = 1;
	sd.BufferDesc.Width = int(m_screenWidth);
	sd.BufferDesc.Height = int(m_screenHeight);
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
		std::cout << "D3D11CreateDeviceAndSwapChain Failed " << hr << std::endl;
		return FALSE;
	}
	std::cout << "D3D11CreateDeviceAndSwapChain Success with FeatureLevel " << m_featureLevel << endl;

	hr = m_pDevice->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
	cout << "CheckMultisampleQualityLevels" << m4xMsaaQuality << endl;
	assert(m4xMsaaQuality > 0);
	return true;
}

bool DirectX11Application::CreateRenderTarget()
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

	// Create DepthStencil View
	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = m_screenWidth;
	depthStencilDesc.Height = m_screenHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	if (mEnable4xMsaa)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	// No MSAA
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = m_pDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	hr = m_pDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	// Bind the view
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRtv, mDepthStencilView);
	return true;
}

void DirectX11Application::SetViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = m_screenWidth;
	viewport.Height = m_screenHeight;

	m_pImmediateContext->RSSetViewports(1, &viewport);
}

void DirectX11Application::BuildGeometryBuffers()
{
	// Create vertex buffer
	HRESULT hr;
	VERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f), XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f) }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(VERTEX) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = vertices;
	hr = m_pDevice->CreateBuffer(&vbd, &vinitData, &m_pVB);


	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVB, &stride, &offset);

	assert(hr == 0, "CreateBufferFailed");

	// Create the index buffer
	WORD indices[] = {
		// 正面
		0, 1, 2,
		2, 3, 0,
		// 左面
		4, 5, 1,
		1, 0, 4,
		// 顶面
		1, 5, 6,
		6, 2, 1,
		// 背面
		7, 6, 5,
		5, 4, 7,
		// 右面
		3, 2, 6,
		6, 7, 3,
		// 底面
		4, 0, 3,
		3, 7, 4
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof indices;
	//ibd.ByteWidth = sizeof(UINT) * 36;

	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	hr = m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB);
	assert(hr == 0, "CreateIndexBuffer Failed");

	m_pImmediateContext->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R16_UINT, 0);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void DirectX11Application::BuildConstantBuffer()
{
	mCBuffer.world = XMMatrixIdentity();	// 单位矩阵的转置是它本身
	mCBuffer.view = XMMatrixTranspose(XMMatrixLookAtLH(
		XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
	));
	mCBuffer.proj = XMMatrixTranspose(XMMatrixPerspectiveFovLH(XM_PIDIV2, AspectRatio(), 1.0f, 1000.0f));

	D3D11_BUFFER_DESC cbd;
	ZeroMemory(&cbd, sizeof(cbd));
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(ConstantBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.CPUAccessFlags = 0;
	HRESULT hr = m_pDevice->CreateBuffer(&cbd, NULL, &m_pCB);

	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pCB);
}

void DirectX11Application::BuildFX()
{

	HRESULT hr;
	ID3DBlob* blob;

	CompileShader(L"Cube_VS.hlsl", "VS", "vs_5_0", &m_pVertexBlob);

	// 创建顶点着色器
	hr = m_pDevice->CreateVertexShader(m_pVertexBlob->GetBufferPointer(), m_pVertexBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	assert(hr == 0);
	// 创建像素着色器
	CompileShader(L"Cube_PS.hlsl", "PS", "ps_5_0", &blob);
	hr = m_pDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &m_pPixelShader);
	assert(hr == 0);


	// ******************
	// 给渲染管线各个阶段绑定好所需资源
	// 将着色器绑定到渲染管线
	m_pImmediateContext->VSSetShader(m_pVertexShader, nullptr, 0);

	m_pImmediateContext->PSSetShader(m_pPixelShader, nullptr, 0);
}

void DirectX11Application::BuildVertexLayout()
{
	HRESULT hr;
	// Create the vertex input layout.
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	// 创建顶点布局
	hr = m_pDevice->CreateInputLayout(vertexDesc, 2, m_pVertexBlob->GetBufferPointer(), m_pVertexBlob->GetBufferSize(), &m_pInputLayout);
	assert(hr == 0);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);
}