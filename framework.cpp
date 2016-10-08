//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "framework.h"

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

	return true;
}

void Framework::OnResize() {
	D3DApp::OnResize();
}

void Framework::UpdateScene(float dt) {
	static FLOAT mPhi = 70.0f;
	static FLOAT mTheta = 0.0f;
	static const FLOAT mRadius = 3.0f;

	// Convert Spherical to Cartesian coordinates.
	float x = mRadius*sinf(mPhi)*cosf(mTheta);
	float z = mRadius*sinf(mPhi)*sinf(mTheta);
	float y = mRadius*cosf(mPhi);

	// Build the model matrix.
	XMMATRIX M = XMMatrixIdentity();

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);

	// Build the project matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(50.0f, (float)mClientWidth / (float)mClientHeight, 0.1f, 10000.0f);

	XMMATRIX MVP = XMMatrixMultiply(XMMatrixMultiply(M, V), P);
	XMStoreFloat4x4(&defaultShader.GetVSConstantData().mvp, MVP);
	
	mTheta += 1.0f;
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
