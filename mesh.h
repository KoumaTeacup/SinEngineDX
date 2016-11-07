#pragma once

#include "asset.h"
#include "d3dUtil.h"

class D3DApp;

using namespace DirectX;

class D3DApp;

#define vertexDataInputCount 6
struct VertexData {
	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 texCoord;
	XMFLOAT3 tangent;
	XMFLOAT3 color;
	UINT useVertexColor;
	VertexData() :
		position(XMFLOAT3()),
		normal(XMFLOAT3()),
		texCoord(XMFLOAT2()),
		tangent(XMFLOAT3()),
		color(XMFLOAT3()),
		useVertexColor(0) {	}
	VertexData(XMFLOAT3 pos) :
		position(pos),
		normal(XMFLOAT3()),
		texCoord(XMFLOAT2()),
		tangent(XMFLOAT3()),
		color(XMFLOAT3()),
		useVertexColor(0) {	}
	VertexData(XMFLOAT3 pos, XMFLOAT3 col) :
		position(pos),
		normal(XMFLOAT3()),
		texCoord(XMFLOAT2()),
		tangent(XMFLOAT3()),
		color(col),
		useVertexColor(1) {
	}
	VertexData(XMFLOAT3 pos, XMFLOAT3 nrm, XMFLOAT2 tex, XMFLOAT3 tan, XMFLOAT3 col, bool useColor = false):
		position(pos),
		normal(nrm),
		texCoord(tex),
		tangent(tan),
		color(col),
		useVertexColor(useColor){}
};

class SEMesh : public SEAsset
{
public:
	SEMesh();
	~SEMesh();

	void Load(const UINT numVertex, const VertexData *meshData, const UINT numIndex = 0, const int *indexData = nullptr);
	void Bind() const;
	void virtual Tick(float dt);
	void virtual Draw();
	void Render();

private:
	bool hasIndex;
	UINT drawCount;
	ID3D11Buffer *buffer;
	ID3D11Buffer *ibuffer;
};