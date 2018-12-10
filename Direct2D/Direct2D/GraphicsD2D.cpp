
#include "GraphicsD2D.h"
#include <math.h>
#include <iostream>
using namespace std;

bool GraphicsD2D::Initialize(HWND &hWnd)
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
		return FALSE;
	hr = pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &pYellowBrush);
	if (FAILED(hr))
		return FALSE;

	if(FAILED(pD2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &pWhiteBrush)))
		return FALSE;
	return TRUE;
}

void GraphicsD2D::Draw()
{
	pD2DRenderTarget->BeginDraw();
	pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DarkGray));

	D2D1_SIZE_F rtSize = pD2DRenderTarget->GetSize();
	//pD2DRenderTarget->DrawRectangle(D2D1::RectF(0 + 10.0f, 0 + 10.0f, rtSize.width - 10.0f, rtSize.height - 10.0f), pBlackBrush);

	// Draw face
	D2D1_POINT_2F center = D2D1::Point2F(rtSize.width / 2, rtSize.height / 2);
	float radius = rtSize.height / 6;
	D2D1_ELLIPSE eclipse_face = D2D1::Ellipse(center, radius, radius);
	pD2DRenderTarget->DrawEllipse(eclipse_face, pYellowBrush);
	pD2DRenderTarget->FillEllipse(eclipse_face, pYellowBrush);

	// Draw mouth
	float center2mouth = 0.3f, mouthrange = 0.3f;
	D2D1_POINT_2F mouthCenter = D2D1::Point2F(center.x, center.y + center2mouth * radius + mouthrange * radius * 0.5);
	D2D1_ELLIPSE eclipse_mouth = D2D1::Ellipse(mouthCenter, mouthrange * radius * 0.5, mouthrange * radius * 0.5 * 0.5);
	pD2DRenderTarget->DrawEllipse(eclipse_mouth, pWhiteBrush);
	pD2DRenderTarget->FillEllipse(eclipse_mouth, pWhiteBrush);

	// Draw eyes
	float eyes_distance = 0.5f, eyes_height = 0.2f, eyes_radius = 0.2f;

	D2D1_POINT_2F left_eye_center = D2D1::Point2F(center.x - eyes_distance * radius, center.y - eyes_height * radius);
	D2D1_ELLIPSE left_eye = D2D1::Ellipse(left_eye_center, eyes_radius * radius, eyes_radius * radius);
	pD2DRenderTarget->DrawEllipse(left_eye, pWhiteBrush);
	pD2DRenderTarget->FillEllipse(left_eye, pWhiteBrush);

	float eyebrow_radius = 0.3;

	float left_theta = asin(float(mouseY - left_eye_center.y) / sqrt(float(mouseX - left_eye_center.x) * (mouseX - left_eye_center.x) + (mouseY - left_eye_center.y) * (mouseY - left_eye_center.y))) ;

	float left_eyebrow_a = 0.8f;
	D2D1_POINT_2F left_eyebrow_center = D2D1::Point2F(left_eye_center.x + left_eyebrow_a * eyes_radius * radius * cos(left_theta), left_eye_center.y + left_eyebrow_a * eyes_radius * radius * sin(left_theta));
	if(mouseX < left_eye_center.x)
		left_eyebrow_center = D2D1::Point2F(left_eye_center.x - left_eyebrow_a * eyes_radius * radius * cos(left_theta), left_eye_center.y + left_eyebrow_a * eyes_radius * radius * sin(left_theta));

	D2D1_ELLIPSE left_eyebrow = D2D1::Ellipse(left_eyebrow_center, eyes_radius * radius * eyebrow_radius, eyes_radius * radius * eyebrow_radius);
	pD2DRenderTarget->DrawEllipse(left_eyebrow, pBlackBrush);
	pD2DRenderTarget->FillEllipse(left_eyebrow, pBlackBrush);

	D2D1_POINT_2F right_eye_center = D2D1::Point2F(center.x + eyes_distance * radius, center.y - eyes_height * radius);
	D2D1_ELLIPSE right_eye = D2D1::Ellipse(right_eye_center, eyes_radius * radius, eyes_radius * radius);
	pD2DRenderTarget->DrawEllipse(right_eye, pWhiteBrush);
	pD2DRenderTarget->FillEllipse(right_eye, pWhiteBrush);

	float right_theta = asin(float(mouseY - right_eye_center.y) / sqrt(float(mouseX - right_eye_center.x) * (mouseX - right_eye_center.x) + (mouseY - right_eye_center.y) * (mouseY - right_eye_center.y)));
	D2D1_POINT_2F right_eyebrow_center = D2D1::Point2F(right_eye_center.x + left_eyebrow_a * eyes_radius * radius * cos(right_theta), left_eye_center.y + left_eyebrow_a * eyes_radius * radius * sin(right_theta));
	if(mouseX < right_eye_center.x)
		right_eyebrow_center = D2D1::Point2F(right_eye_center.x - left_eyebrow_a * eyes_radius * radius * cos(right_theta), left_eye_center.y + left_eyebrow_a * eyes_radius * radius * sin(right_theta));

	D2D1_ELLIPSE right_eyebrow = D2D1::Ellipse(right_eyebrow_center, eyes_radius * radius * eyebrow_radius, eyes_radius * radius * eyebrow_radius);
	pD2DRenderTarget->DrawEllipse(right_eyebrow, pBlackBrush);
	pD2DRenderTarget->FillEllipse(right_eyebrow, pBlackBrush);


	pD2DRenderTarget->EndDraw();
}


void GraphicsD2D::OnMouseMove(int x, int y)
{
	mouseX = x, mouseY = y;
	Draw();
}