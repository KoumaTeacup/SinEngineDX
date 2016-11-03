//***************************************************************************************
// Init Direct3D.cpp by Frank Luna (C) 2011 All Rights Reserved.
//
// Demonstrates the sample framework by initializing Direct3D, clearing 
// the screen, and displaying frame stats.
//
//***************************************************************************************

#include "framework.h"
#include "mesh.h"
#include "bone.h"
#include "skeleton.h"

using namespace DirectX;

SEShader Framework::currentShader = SEShader();
XMMATRIX Framework::currentVP = XMMATRIX();
SERenderMode Framework::currentMode = SE_RENDER_DEFAULT;
SERenderStates Framework::renderState = SERenderStates();

Framework::Framework(HINSTANCE hInstance)
	: D3DApp(hInstance),
	paused(false),
	mouseX(-1),
	mouseY(-1),
	cameraPhi(XM_PI * 0.4f),
	cameraTheta(0.0f),
	cameraRadius(5.0f),
	cameraFocus(0.0f, 0.0f, 0.0f){
	mClientHeight = 1080;
	mClientWidth = 1920;

}

Framework::~Framework() {
	//for (auto mesh : meshes) delete mesh;
	//for (auto skeleton : skeletons) delete skeleton;
	for (auto i : assets) delete i;
}

bool Framework::Init() {
	if (!D3DApp::Init())
		return false;

	currentShader.Init("Default");
	SEBone::InitBoneMesh();
	renderState.Init();

	return true;
}

void Framework::OnResize() {
	D3DApp::OnResize();
}

void Framework::UpdateScene(float dt) {

	// Build the view matrix.
	XMMATRIX V = XMMatrixLookAtLH(
		// Camera Position
		XMVectorSet(
			cameraRadius*sinf(cameraPhi)*sinf(cameraTheta),	// x
			cameraRadius*cosf(cameraPhi),					// y
			cameraRadius*sinf(cameraPhi)*-cosf(cameraTheta),// z
			0.0f)
		+ XMLoadFloat3(&cameraFocus),
		// Camera Focus
		XMLoadFloat3(&cameraFocus),
		// Camera Up Vector
		XMVectorSet(0.0f, sinf(cameraPhi), 0.0f, 0.0f)
	);

	// Build the project matrix
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)mClientWidth / (float)mClientHeight, 0.1f, 1000.0f);

	// Save VP for scene
	currentVP = XMMatrixIdentity() * V * P;

	// Tick bone animations
	//for (auto i : skeletons) if(!paused) i->Tick();
	for (auto i : assets) if (!paused) i->Tick();
}

void Framework::DrawScene() {

	SEContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	SEContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	SEContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Concatenate Model - View - Projection matrix
	XMStoreFloat4x4(&currentShader.GetVSConstantData().mvp, XMMatrixTranspose(currentVP));

	currentShader.Bind();

	for (auto i : assets) {
		i->Draw();
	}

	mSwapChain->Present(0, 0);
}

void Framework::loadExplicitMesh(const UINT numVertex, const VertexData * meshData, const int numIndex, const int * indexData) {
	SEMesh *newMesh = new SEMesh(numVertex, meshData, numIndex, indexData);
	newMesh->setAssetName("defaultCube");
	newMesh->setType(SE_ASSET_MESH);
	assets.push_back(newMesh);
}

void Framework::loadFBX(const char * filename) {
	auto importedAssets = fbxManager.importScene(filename);
	for (auto i : importedAssets) {
		assets.push_back(i);
	/*	switch (i->getType()) {
		case SE_ASSET_SKELETON:
			skeletons.push_back(static_cast<SESkeleton*>(i));
		case SE_ASSET_MESH:
			meshes.push_back(static_cast<SEMesh*>(i));
		}*/
	}
}

LRESULT Framework::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	D3DApp::MsgProc(hwnd, msg, wParam, lParam);

	switch (msg) {
	case WM_KEYDOWN:
		switch (wParam) {
		case 0x46: // F-Key
			cameraFocus = XMFLOAT3(0.0f, 0.0f, 0.0f);
			break;
		case VK_SPACE:
			if (paused) {
				paused = false;
			}
			else {
				paused = true;
			}
			break;
		case VK_F2:
			renderState.Set(SE_RENDER_WIREFRAME);
			break;
		case VK_F3:
			renderState.Set(SE_RENDER_PREPARE_DISPLAY_BONES);
			break;
		case VK_F4:
			renderState.Set(SE_RENDER_DISABLE_FACE_CULLING);
			break;
		case VK_F5:
		case VK_F6:
		case VK_F7:
		case VK_F8:
		case VK_F9:
		case VK_F10:
		case VK_F11:
		case VK_F12:
		case VK_F1: 
			renderState.Set(SE_RENDER_DEFAULT);
			break;
		}
	case WM_MOUSEWHEEL:
		cameraRadius -= GET_WHEEL_DELTA_WPARAM(wParam) * 0.0015f * cameraRadius;
		break;

	default:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Framework::OnMouseMove(WPARAM btnState, int x, int y) {
	switch (btnState) {
	case MK_LBUTTON:
	{
		cameraPhi -= (y - mouseY) * 0.005f;
		cameraTheta -= (x - mouseX) * 0.005f;
		break;
	}
	case MK_RBUTTON:

		cameraFocus.x += (
			(mouseX - x) * cosf(cameraTheta) + 
			(mouseY - y) * cosf(cameraPhi) * sinf(cameraTheta)
			) * 0.0015f * cameraRadius;

		cameraFocus.z += (
			(mouseX - x) * sinf(cameraTheta) -
			(mouseY - y) * cosf(cameraPhi) * cosf(cameraTheta)
			) * 0.0015f * cameraRadius;

		cameraFocus.y += (y - mouseY) *0.0015f * sinf(cameraPhi) * cameraRadius;
		break;
	default:
		break;
	}
	mouseX = x;
	mouseY = y;
}