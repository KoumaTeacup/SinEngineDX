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

	framework.loadFBX("fbx/Shin_SimpleScene_CleanModel.fbx");
	//framework.loadExplicitCurve(simpleCurveNum, &simpleCurve[0], "simpleCurve");
	//framework.loadExplicitCurve(testCurveNum, &testCurve[0], "testCurve");
	//framework.loadFBX("fbx/sylvanas_run_only.fbx");
	//framework["fbx/sylvanas_run_only:ladysylvanaswindrunner"]->getMovement().AssignCurve(
	//	static_cast<SESpline*>(framework["testCurve"]));
	//framework["fbx/sylvanas_run_only:ladysylvanaswindrunner"]->getMovement().EnablePath();

	//framework.loadFBX("fbx/ikModel.fbx");
	//framework["fbx/ikModel:joint1"]->getMovement().AssignCurve(
	//	static_cast<SESpline*>(framework["ikPathCurve"]));
	//framework["sePolygon1"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));

	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "anchorLeft");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "anchorRight");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "Box0");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "Box1");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "Box2");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "Box3");
	framework.loadExplicitMesh(boxVCount, boxV, boxICount, boxI, "Box4");
	framework["anchorLeft"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(-120.0f, 100.0f, 0.0f, 0.0f));
	framework["anchorRight"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(120.0f, 100.0f, 0.0f, 0.0f));
	framework["Box0"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(-80.0f, 100.0f, 0.0f, 0.0f));
	framework["Box0"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));
	framework["Box0"]->enablePhysics();
	framework["Box1"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(-40.0f, 100.0f, 0.0f, 0.0f));
	framework["Box1"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));
	framework["Box1"]->enablePhysics();
	framework["Box2"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(0.0f, 100.0f, 0.0f, 0.0f));
	framework["Box2"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));
	framework["Box2"]->enablePhysics();
	framework["Box3"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(40.0f, 100.0f, 0.0f, 0.0f));
	framework["Box3"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));
	framework["Box3"]->enablePhysics();
	framework["Box4"]->getMovement().SetWorldTranslate(DirectX::XMVectorSet(80.0f, 100.0f, 0.0f, 0.0f));
	framework["Box4"]->getMovement().SetScale(DirectX::XMVectorSet(5.0f, 5.0f, 5.0f, 0.0f));
	framework["Box4"]->enablePhysics();
	framework.loadSpring(framework["anchorLeft"], 0, framework["Box0"], 0);
	framework.loadSpring(framework["Box0"], 3, framework["Box1"], 0);
	framework.loadSpring(framework["Box1"], 7, framework["Box2"], 6);
	framework.loadSpring(framework["Box2"], 4, framework["Box3"], 7);
	framework.loadSpring(framework["Box3"], 0, framework["Box4"], 5);
	framework.loadSpring(framework["anchorRight"], 0, framework["Box4"], 7);

	return framework.Run();
}