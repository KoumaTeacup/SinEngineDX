#pragma once

#include "d3dApp.h"
#include "FBX.h"
#include "shader.h"
#include "states.h"
#include <unordered_map>

#define SE_Shader	Framework::currentShader
#define SE_VP		Framework::currentVP
#define SE_Mode		Framework::currentMode
#define SE_State	Framework::renderState

class SEAsset;

struct VertexData;
struct CurveControlPoint;

enum SERenderMode
{
	SE_RENDER_DEFAULT,				// F1
	SE_RENDER_WIREFRAME,			// F2
	SE_RENDER_DISPLAY_BONES,		// F3
	SE_RENDER_DISABLE_FACE_CULLING,	// F4

	SE_RENDER_PREPARE_DISPLAY_BONES,// reserved for internal use
};

class Framework : public D3DApp
{
public:
	Framework(HINSTANCE hInstance);
	~Framework();

	SEAsset *operator[](std::string name) { return assets[name]; }

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void loadExplicitMesh(const UINT num, 
		const VertexData * meshData,
		const int numIndex = 0, 
		const int *indexData = nullptr,
		std::string assetName = "sePolygon");
	void loadExplicitCurve(const UINT num, const CurveControlPoint *CPs, std::string assetName = "seSpline");
	void loadFBX(const char* filename);

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ID3D11Device * getDevice() { return gDivece; }
	ID3D11DeviceContext * getContext() { return gContext; }

	static SEShader *currentShader;
	static DirectX::XMMATRIX currentVP;
	static SERenderMode currentMode;
	static SERenderStates renderState;

protected:
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

private:
	static ID3D11Device *gDivece;
	static ID3D11DeviceContext *gContext;

	bool paused;

	SEFBX fbxManager;

	//std::vector<SESkeleton*> skeletons;
	//std::vector<SEMesh*> meshes;

	std::unordered_map<std::string, SEAsset*> assets;

	int mouseX, mouseY;
	FLOAT cameraPhi;
	FLOAT cameraTheta;
	FLOAT cameraRadius;
	DirectX::XMFLOAT3 cameraFocus;

	SEShader defaultSH, curveSH;
};