#pragma once

#include "d3dUtil.h"

class D3DApp;

using namespace DirectX;

class D3DApp;

#define vertexDataInputCount 4
struct VertexData {
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;
	XMFLOAT3 tangent;
	VertexData(XMFLOAT3 pos):position(pos){}
};

class SEVAO
{
public:
	SEVAO(D3DApp *_framework, UINT num, VertexData *data);
	~SEVAO();

	void Load(UINT num, VertexData *data);
	void Unload();
	void Bind();
	void Draw();

private:
	D3DApp * framework;
	UINT vertexCount;
	ID3D11Buffer *buffer;
};