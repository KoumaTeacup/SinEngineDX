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
		VertexData(XMFLOAT3(-1.f, -1.f, -1.f)),	VertexData(XMFLOAT3(-1.f, 1.f, -1.f)), VertexData(XMFLOAT3(1.f, 1.f, -1.f)),
		VertexData(XMFLOAT3(-1.f, -1.f, -1.f)),	VertexData(XMFLOAT3(1.f, 1.f, -1.f)), VertexData(XMFLOAT3(1.f, -1.f, -1.f)),
		// back
		VertexData(XMFLOAT3(-1.f, 1.f, 1.f)), VertexData(XMFLOAT3(-1.f, -1.f, 1.f)), VertexData(XMFLOAT3(1.f, -1.f, 1.f)),
		VertexData(XMFLOAT3(-1.f, 1.f, 1.f)), VertexData(XMFLOAT3(1.f, -1.f, 1.f)),	VertexData(XMFLOAT3(1.f, 1.f, 1.f)),
		// top
		VertexData(XMFLOAT3(-1.f, 1.f, -1.f)), VertexData(XMFLOAT3(-1.f, 1.f, 1.f)), VertexData(XMFLOAT3(1.f, 1.f, 1.f)),
		VertexData(XMFLOAT3(-1.f, 1.f, -1.f)), VertexData(XMFLOAT3(1.f, 1.f, 1.f)), VertexData(XMFLOAT3(1.f, 1.f, -1.f)),
		// bottom
		VertexData(XMFLOAT3(-1.f, -1.f, 1.f)), VertexData(XMFLOAT3(-1.f, -1.f, -1.f)), VertexData(XMFLOAT3(1.f, -1.f, -1.f)),
		VertexData(XMFLOAT3(-1.f, -1.f, 1.f)), VertexData(XMFLOAT3(1.f, -1.f, -1.f)), VertexData(XMFLOAT3(1.f, -1.f, 1.f)),
		// left
		VertexData(XMFLOAT3(-1.f, -1.f, 1.f)), VertexData(XMFLOAT3(-1.f, 1.f, 1.f)), VertexData(XMFLOAT3(-1.f, 1.f, -1.f)),
		VertexData(XMFLOAT3(-1.f, -1.f, 1.f)), VertexData(XMFLOAT3(-1.f, 1.f, -1.f)), VertexData(XMFLOAT3(-1.f, -1.f, -1.f)),
		// right
		VertexData(XMFLOAT3(1.f, -1.f, -1.f)), VertexData(XMFLOAT3(1.f, 1.f, -1.f)), VertexData(XMFLOAT3(1.f, 1.f, 1.f)),
		VertexData(XMFLOAT3(1.f, -1.f, -1.f)), VertexData(XMFLOAT3(1.f, 1.f, 1.f)), VertexData(XMFLOAT3(1.f, -1.f, 1.f))
	};

	framework.loadMesh(ARRAYSIZE(boxData), boxData);

	return framework.Run();
}