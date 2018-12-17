#include "DX11Graphics.h"
#include "DirectXMath.h"
#include "DirectXPackedVector.h"
#include "DirectXColors.h"
#include <iostream>
#include <d3dcompiler.h>

using namespace std;
using namespace DirectX;
using namespace DirectX::PackedVector;

struct VERTEX {
	XMFLOAT3    Position;
	XMFLOAT4    Color;
};


SIZE DX11Graphics::WindowSize()
{
	RECT rt;
	GetWindowRect(m_hwnd, &rt);

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
	SetViewport();
	cout << "SetViewPort Success" << endl;
	InitPipeline();
	cout << "InitPipeline Success" << endl;
	InitGraphics();
	cout << "InitGraphics Success" << endl;
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

void DX11Graphics::InitPipeline()
{
	// load and compile the two shaders
	ID3DBlob *VS, *PS;

	D3DReadFileToBlob(L"copy.vso", &VS);
	D3DReadFileToBlob(L"copy.pso", &PS);

	// encapsulate both shaders into shader objects
	m_pDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVS);
	m_pDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPS);

	// set the shader objects
	m_pImmediateContext->VSSetShader(m_pVS, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPS, 0, 0);

	// create the input layout object
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	m_pDevice->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	VS->Release();
	PS->Release();
}

void DX11Graphics::InitGraphics()
{
	// create a triangle using the VERTEX struct
	VERTEX OurVertices[] =
	{
		{XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(0.45f, -0.5, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	// create the vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
	bd.ByteWidth = sizeof(VERTEX) * 3;             // size is the VERTEX struct * 3
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

	m_pDevice->CreateBuffer(&bd, NULL, &m_pBuffer);       // create the buffer

	// copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;
	m_pImmediateContext->Map(m_pBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
	memcpy(ms.pData, OurVertices, sizeof(VERTEX) * 3);                       // copy the data
	m_pImmediateContext->Unmap(m_pBuffer, NULL);                                      // unmap the buffer
}

void DX11Graphics::OnDraw()
{
	// clear the back buffer to a deep blue
	const FLOAT clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRtv, clearColor);

	// do 3D rendering on the back buffer here
	{
		// select which vertex buffer to display
		UINT stride = sizeof(VERTEX);
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pBuffer, &stride, &offset);

		// select which primtive type we are using
		m_pImmediateContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// draw the vertex buffer to the back buffer
		m_pImmediateContext->Draw(3, 0);
	}

	// swap the back buffer and the front buffer
	m_pSwapChain->Present(0, 0);
}