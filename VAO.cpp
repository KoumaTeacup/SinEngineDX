#include "VAO.h"
#include "d3dApp.h"

SEVAO::SEVAO(D3DApp *_framework, UINT num, VertexData *data) :framework(_framework), buffer(nullptr)
{
	Load(num, data);
}

SEVAO::~SEVAO() {
	ReleaseCOM(buffer);
}

void SEVAO::Load(UINT num, VertexData *data)
{
	vertexCount = num;

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VertexData) * vertexCount;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = 0;
	bd.MiscFlags = 0;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = data;
	sd.SysMemPitch = 0;
	sd.SysMemSlicePitch = 0;

	framework->md3dDevice->CreateBuffer(&bd, &sd, &buffer);
}

void SEVAO::Unload() {
	ReleaseCOM(buffer);
}

void SEVAO::Bind()
{
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	framework->md3dContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void SEVAO::Draw()
{
	framework->md3dContext->Draw(vertexCount, 0);
}
