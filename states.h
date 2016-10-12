#pragma once

#include "d3dUtil.h"

class D3DApp;
enum SERenderMode;

class SERenderStates
{
public:
	SERenderStates(D3DApp *_framework) :
		framework(_framework), 
		standard(nullptr),
		wireframe(nullptr){}

	void Init();
	void Set(SERenderMode mode);
private:
	D3DApp *framework;
	ID3D11RasterizerState *standard;
	ID3D11RasterizerState *wireframe;
	ID3D11RasterizerState *noFaceCulling;
};