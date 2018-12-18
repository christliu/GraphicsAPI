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

void DX11Graphics::BuildGeometryBuffers()
{
	// Create vertex buffer
	HRESULT hr;
	VERTEX vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)Colors::White) },
	{ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Black) },
	{ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Red) },
	{ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4((const float*)&Colors::Green) },
	{ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Blue) },
	{ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Yellow) },
	{ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Cyan) },
	{ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4((const float*)&Colors::Magenta) }
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
	assert(hr == 0, "CreateBufferFailed");

	// Create the index buffer

	UINT indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = indices;
	HR(m_pDevice->CreateBuffer(&ibd, &iinitData, &m_pIB));
}


void DX11Graphics::OnDraw()
{
	// clear the back buffer to a deep blue
	const FLOAT clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRtv, clearColor);
	cout << "First Branch CheckOut" << endl;

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