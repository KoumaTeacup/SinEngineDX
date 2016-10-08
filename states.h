#pragma once

#include "d3dUtil.h"

class D3DApp;

class SERenderStates
{
public:
	SERenderStates(D3DApp *_framework) :framework(_framework), state(nullptr) {}

	void Init();
	void Set();
private:
	D3DApp *framework;
	ID3D11RasterizerState *state;
};