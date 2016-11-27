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
#include "spline.h"

using namespace DirectX;

SEShader *Framework::currentShader = nullptr;
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
	for (auto i : assets) delete i.second;
}

bool Framework::Init() {
	if (!D3DApp::Init())
		return false;

	defaultSH.Init("Default", SE_INPUT_LAYOUT_VERTEX_DATA);
	curveSH.Init("Curve", SE_INPUT_LAYOUT_CURVE_DATA);
	currentShader = &defaultSH;
	//currentShader = &curveSH;
	SEBone::InitBoneMesh();
	renderState.Init();

	assets["ikPathCurve"] = new SESpline();

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
	XMMATRIX P = XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)mClientWidth / (float)mClientHeight, 0.1f, 10000.0f);

	// Save VP for scene
	currentVP = XMMatrixIdentity() * V * P;

	// Tick bone animations
	//for (auto i : skeletons) if(!paused) i->Tick();
	if(!paused) {
		for(auto i : assets) {
			if(!i.second) continue;
			switch(i.second->getType()) {
			case SE_ASSET_MESH:
				currentShader = &defaultSH;
				i.second->Tick(dt);
				break;
			case SE_ASSET_SKELETON:
				currentShader = &defaultSH;
				i.second->Tick(dt);
				static_cast<SESkeleton*>(i.second)->resolveIK(assets["sePolygon1"]->getMovement().getWorldTranslate(), dt);
				break;
			case SE_ASSET_SPLINE:
				currentShader = &curveSH;
				i.second->Tick(dt);
				break;
			default:
				break;
			}
		}
	}
}

void Framework::DrawScene() {

	SEContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
	SEContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
	SEContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	for (auto i : assets) {
		if(!i.second) continue;
		switch(i.second->getType()) {
		case SE_ASSET_MESH:
			defaultSH.Bind();
			currentShader = &defaultSH;
			break;
		case SE_ASSET_SKELETON:
			defaultSH.Bind();
			currentShader = &defaultSH;
			break;
		case SE_ASSET_SPLINE:
			curveSH.Bind();
			currentShader = &curveSH;
			break;
		default:
			break;
		}
		i.second->Draw();
	}

	mSwapChain->Present(0, 0);
}

void Framework::loadExplicitMesh(const UINT numVertex, const VertexData * meshData, const int numIndex, const int * indexData, std::string assetName) {
	if(assetName == "sePolygon") assetName += '0' + assets.size();
	SEMesh *newMesh = new SEMesh();
	newMesh->Load(numVertex, meshData, numIndex, indexData);
	newMesh->setAssetName(assetName);
	assets[assetName] = newMesh;
}


void Framework::loadExplicitCurve(const UINT num, const CurveControlPoint * CPs, std::string assetName)
{
	if(assetName == "seSpline") assetName += '0' + assets.size();
	SESpline *newSpline = new SESpline();
	newSpline->loadCP(num, CPs);
	newSpline->setAssetName(assetName);
	assets[assetName] = newSpline;
}

void Framework::loadFBX(const char * filename) {
	fbxManager.importScene(filename, assets);
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

	case MK_CONTROL:
		assets["sePolygon1"]->getMovement().AddWorldTranslate(XMVectorSet(0.0f, mouseY-y, 0.0f, 0.0f));
		updatePolygonPath();
		break;
	default:
		assets["sePolygon1"]->getMovement().AddWorldTranslate(XMVectorSet(x - mouseX, 0.0f, mouseY - y, 0.0f));
		//assets["sePolygon1"]->getMovement().SetWorldTranslate(XMVectorSet(50.0f, 50.0f, 50.0f, 0.0f));
		//assets["sePolygon1"]->getMovement().AddWorldTranslate(XMVectorSet(x - mouseX, 0.0f, mouseY - y, 0.0f));
		updatePolygonPath();
		break;
	}
	mouseX = x;
	mouseY = y;
}

void Framework::updatePolygonPath() {
	SESpline *pCurve = static_cast<SESpline*>(assets["ikPathCurve"]);

	XMVECTOR modelPos = XMLoadFloat3(&static_cast<SESkeleton*>(assets["fbx/ikModel:joint1"])->rootTrans) * XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR targetPos = assets["sePolygon1"]->getMovement().getWorldTranslate() * XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);

	const CurveControlPoint path[4] = {
		CurveControlPoint(modelPos),
		CurveControlPoint(modelPos + (targetPos - modelPos) * XMVectorSet(0.23f, 0.0f, 0.43f, 0.0f)),
		CurveControlPoint(modelPos + (targetPos - modelPos) * XMVectorSet(0.76f, 0.0f, 0.56f, 0.0f)),
		CurveControlPoint(targetPos) };
	pCurve->loadCP(4, path);

	assets["fbx/ikModel:joint1"]->getMovement().tableReady = false;
	assets["fbx/ikModel:joint1"]->getMovement().Reset();
}
