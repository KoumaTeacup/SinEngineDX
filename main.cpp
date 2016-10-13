#include "framework.h"

#include "meshes.h"
#include "VQS.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd) {

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Framework framework(hInstance);

	if (!framework.Init())	return 0;

	framework.loadMesh(boxVCount, boxV, boxICount, boxI);

	return framework.Run();
}