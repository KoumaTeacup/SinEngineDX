#pragma once

#include "d3dApp.h"
#include "FBX.h"

#define SE_Shader	Framework::currentShader
#define SE_VP		Framework::currentVP
#define SE_Mode		Framework::currentMode
#define SE_State	Framework::renderState

class SEScene;

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

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void loadMesh(const UINT num, const VertexData * meshData, const int numIndex = 0, const int *indexData = nullptr);
	void loadFBX(const char* filename);

	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	ID3D11Device * getDevice() { return gDivece; }
	ID3D11DeviceContext * getContext() { return gContext; }

	static SEShader currentShader;
	static XMMATRIX currentVP;
	static SERenderMode currentMode;
	static SERenderStates renderState;

protected:
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

private:
	static ID3D11Device *gDivece;
	static ID3D11DeviceContext *gContext;

	bool changeMode;

	SEFBX fbxManager;
	std::vector<SEScene*> scenes;

	std::vector<SEVAO*> meshes;

	int mouseX, mouseY;
	FLOAT cameraPhi;
	FLOAT cameraTheta;
	FLOAT cameraRadius;
	XMFLOAT3 cameraFocus;


};