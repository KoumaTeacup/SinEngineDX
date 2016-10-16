#pragma once

#include "d3dUtil.h"

class D3DApp;
enum SERenderMode;

class SERenderStates
{
public:
	SERenderStates();

	void Init();
	void Set(SERenderMode mode);
	void Restore();
private:
	SERenderMode saved;

	ID3D11RasterizerState *standard;
	ID3D11RasterizerState *wireframe;
	ID3D11RasterizerState *noFaceCulling;
	ID3D11RasterizerState *xrayBones;
};