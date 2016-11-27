#pragma once

#include "framework.h"
#include "meshPresets.h"
#include "curvePresets.h"

//#include <vld.h>

using namespace SE_Internal_Mesh;
using namespace SE_Internal_Curve;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd) {

	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	Framework framework(hInstance);

	if (!framework.Init())	return 0;

	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI);
	framework.loadFBX("fbx/Shin_SimpleScene_CleanModel.fbx");
	//framework.loadExplicitCurve(simpleCurveNum, &simpleCurve[0], "simpleCurve");
	//framework.loadExplicitCurve(testCurveNum, &testCurve[0], "testCurve");
	//framework.loadFBX("fbx/sylvanas_run_only.fbx");
	//framework["fbx/sylvanas_run_only:ladysylvanaswindrunner"]->getMovement().AssignCurve(
	//	static_cast<SESpline*>(framework["testCurve"]));
	//framework["fbx/sylvanas_run_only:ladysylvanaswindrunner"]->getMovement().EnablePath();

	framework.loadFBX("fbx/ikModel.fbx");
	framework["fbx/ikModel:joint1"]->getMovement().AssignCurve(
		static_cast<SESpline*>(framework["ikPathCurve"]));
	framework["sePolygon1"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));

	return framework.Run();
}