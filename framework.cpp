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
	defaultState(this),
	defaultShader(this) {
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

	defaultShader.Init("Default");
	defaultState.Init();
	defaultState.Set();

	return true;
}

void Framework::OnResize() {
	D3DApp::OnResize();
}

void Framework::UpdateScene(float dt) {
	static FLOAT mPhi = XM_PI * 0.4f;
	static FLOAT mTheta = 0.0f;
	static const FLOAT mRadius = 10.0f;

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the model matrix.
	XMMATRIX M = XMMatrixIdentity();

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 0.0f);
	pos = XMVectorSet(0.0f, 0.1f, 0.0f, 0.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);

	M = XMMatrixTranslation(0.f, 0.f, 2.f * sinf(mTheta));
	M = XMMatrixTranslation(0.5f, 0.0f, 0.f);
	//M = XMMatrixRotationY(mTheta) * XMMatrixTranslation(0.f, 0.f, 0.5f);

	// Build the project matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)mClientWidth / (float)mClientHeight, 0.1f, 1000.0f);

	// Concatenate Model - View - Projection matrix
	XMMATRIX MVP = XMMatrixMultiply(XMMatrixMultiply(M, V), P);
	XMStoreFloat4x4(&defaultShader.GetVSConstantData().mvp, XMMatrixTranspose(M));
	//XMStoreFloat4x4(&defaultShader.GetVSConstantData().mvp, XMMatrixIdentity());
	
	mTheta += dt;
}

void Framework::DrawScene() {

	md3dContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	md3dContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Blue));
	md3dContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	defaultShader.Bind();

	for (auto mesh : meshes) {
		mesh->Bind();
		mesh->Draw();
	}

	mSwapChain->Present(0, 0);
}

void Framework::loadMesh(UINT num, VertexData * data) {
	SEVAO *newMesh = new SEVAO(this, num, data);
	meshes.push_back(newMesh);
}
