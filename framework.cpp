//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "framework.h"

using namespace DirectX;

Framework::Framework(HINSTANCE hInstance)
	: D3DApp(hInstance),
	mode(SE_RENDER_DEFAULT),
	changeMode(false),
	renderState(this),
	defaultShader(this),
	mouseX(-1),
	mouseY(-1),
	cameraPhi(XM_PI * 0.4f),
	cameraTheta(0.0f),
	cameraRadius(5.0f) {
	mClientHeight = 1080;
	mClientWidth = 1920;
}

Framework::~Framework() {
	for (auto mesh : meshes) {
		delete mesh;
	}
}

bool Framework::Init() {
	if (!D3DApp::Init())
		return false;

	// Assuming using Triangle List the entire demo
	md3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//md3dContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	defaultShader.Init("Default");
	renderState.Init();

	return true;
}

void Framework::OnResize() {
	D3DApp::OnResize();
}

void Framework::UpdateScene(float dt) {

	// Convert Camera Spherical to Cartesian coordinates.
	float x = cameraRadius*sinf(cameraPhi)*cosf(cameraTheta);
	float z = cameraRadius*sinf(cameraPhi)*sinf(cameraTheta);
	float y = cameraRadius*cosf(cameraPhi);

	// Build the model matrix.
	XMMATRIX M = XMMatrixIdentity();

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 0.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);

	// Build the project matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)mClientWidth / (float)mClientHeight, 0.1f, 1000.0f);

	// Concatenate Model - View - Projection matrix
	XMStoreFloat4x4(&defaultShader.GetVSConstantData().mvp, XMMatrixTranspose(M*V*P));

}

void Framework::DrawScene() {

	md3dContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	md3dContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	md3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	defaultShader.Bind();

	for (auto mesh : meshes) {
		mesh->Bind();
		mesh->Draw();
	}

	if (mode == SE_RENDER_DISPLAY_BONES) {
		for (auto i : scenes) {
			
		}
	}

	mSwapChain->Present(0, 0);
}

void Framework::loadMesh(const UINT numVertex, const VertexData * meshData, const int numIndex, const int * indexData) {
	SEVAO *newMesh = new SEVAO(this, numVertex, meshData, numIndex, indexData);
	meshes.push_back(newMesh);
}

void Framework::loadFBX(const char * filename) {
	scenes.push_back(fbxManager.importScene(filename));
}

LRESULT Framework::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	D3DApp::MsgProc(hwnd, msg, wParam, lParam);

	switch (msg) {
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_F2:
			mode = SE_RENDER_WIREFRAME; 
			renderState.Set(SE_RENDER_WIREFRAME);
			changeMode = true;
			break;
		case VK_F3:
			mode = SE_RENDER_DISPLAY_BONES;
			renderState.Set(SE_RENDER_DISPLAY_BONES);
			changeMode = true;
			break;
		case VK_F4:
			mode = SE_RENDER_DISABLE_FACE_CULLING;
			renderState.Set(SE_RENDER_DISABLE_FACE_CULLING);
			changeMode = true;
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_F1: 
			mode = SE_RENDER_DEFAULT;
			renderState.Set(SE_RENDER_DEFAULT);
			changeMode = true;
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Framework::OnMouseMove(WPARAM btnState, int x, int y) {
	// ignore the first input
	if (mouseX == -1) {
		mouseX = x;
		mouseY = y;
		return;
	}

	switch (btnState) {
	case MK_LBUTTON:
		cameraTheta -= (x - mouseX) * 0.005f;
		cameraPhi -= (y - mouseY) * 0.005f;
		break;
	case MK_RBUTTON:
		cameraRadius -= (x - mouseX) * 0.01f;
		break;
	default:
		break;
	}
	mouseX = x;
	mouseY = y;
}
