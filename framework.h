#include "d3dApp.h"
#include "FBX.h"

class SEScene;

enum SERenderMode
{
	SE_RENDER_DEFAULT,				// F1
	SE_RENDER_WIREFRAME,			// F2
	SE_RENDER_DISPLAY_BONES,		// F3
	SE_RENDER_DISABLE_FACE_CULLING,	// F4
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
protected:
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
private:
	SERenderMode mode;
	bool changeMode;

	SEFBX fbxManager;
	std::vector<SEScene*> scenes;

	SERenderStates renderState;
	SEShader defaultShader;
	std::vector<SEVAO*> meshes;

	int mouseX, mouseY;
	FLOAT cameraPhi;
	FLOAT cameraTheta;
	FLOAT cameraRadius;


};