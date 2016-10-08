#include "d3dApp.h"

class Framework : public D3DApp
{
public:
	Framework(HINSTANCE hInstance);
	~Framework();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

	void loadMesh(UINT num, VertexData * data);
private:
	SERenderStates defaultState;
	SEShader defaultShader;
	std::vector<SEVAO*> meshes;
};