#include "framework.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd) {
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Framework framework(hInstance);

	if (!framework.Init())
		return 0;

	VertexData boxData[36] = {
		// front
		VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),	VertexData(XMFLOAT3(-0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, -0.5f)),
		VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),	VertexData(XMFLOAT3(0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)),
		// back
		VertexData(XMFLOAT3(-0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(-0.5f, -0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, 0.5f)),
		VertexData(XMFLOAT3(-0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, 0.5f)),	VertexData(XMFLOAT3(0.5f, 0.5f, 0.5f)),
		// top
		VertexData(XMFLOAT3(-0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(-0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, 0.5f)),
		VertexData(XMFLOAT3(-0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, -0.5f)),
		// bottom
		VertexData(XMFLOAT3(-0.5f, -0.5f, 0.5f)), VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)),
		VertexData(XMFLOAT3(-0.5f, -0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, 0.5f)),
		// left
		VertexData(XMFLOAT3(-0.5f, -0.5f, 0.5f)), VertexData(XMFLOAT3(-0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(-0.5f, 0.5f, -0.5f)),
		VertexData(XMFLOAT3(-0.5f, -0.5f, 0.5f)), VertexData(XMFLOAT3(-0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),
		// right
		VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, 0.5f)),
		VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)), VertexData(XMFLOAT3(0.5f, 0.5f, 0.5f)), VertexData(XMFLOAT3(0.5f, -0.5f, 0.5f))
	};

	VertexData triangleData[3] = {
		VertexData(XMFLOAT3(-0.5f, -0.5f, 0.f)), VertexData(XMFLOAT3(0.f, 0.5f, 0.f)), VertexData(XMFLOAT3(0.5f, -0.5f, 0.f))
	};

	VertexData tetrahedronData[12] = {
		VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),	VertexData(XMFLOAT3(0.f, 0.5f, 0.0f)), VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)),
		VertexData(XMFLOAT3(0.f, -0.5f, 0.5f)),	VertexData(XMFLOAT3(0.f, 0.5f, 0.0f)), VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),
		VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)),	VertexData(XMFLOAT3(0.f, 0.5f, 0.0f)), VertexData(XMFLOAT3(0.f, -0.5f, 0.5f)),
		VertexData(XMFLOAT3(-0.5f, -0.5f, -0.5f)),	VertexData(XMFLOAT3(0.5f, -0.5f, -0.5f)), VertexData(XMFLOAT3(0.f, -0.5f, 0.5f))
	};

	framework.loadMesh(ARRAYSIZE(triangleData), triangleData);

	return framework.Run();
}