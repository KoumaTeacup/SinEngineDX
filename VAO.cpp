#include "VAO.h"
#include "d3dApp.h"

SEVAO::SEVAO(D3DApp *_framework, const UINT numVertex, const VertexData *meshData, const UINT numIndex, const int *indexData) :
	framework(_framework), 
	buffer(nullptr), 
	ibuffer(nullptr),
	hasIndex(false),
	drawCount(0)
{
	Load(numVertex, meshData, numIndex, indexData);
}

SEVAO::~SEVAO() {
	ReleaseCOM(buffer);
	if (hasIndex) ReleaseCOM(ibuffer);
}

void SEVAO::Load(const UINT numVertex, const VertexData *meshData, const UINT numIndex, const int *indexData)
{
	if (buffer) ReleaseCOM(buffer);
	if (ibuffer)ReleaseCOM(ibuffer);
	hasIndex = false;

	drawCount = numVertex;

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(VertexData) * numVertex;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	sd.pSysMem = meshData;

	framework->md3dDevice->CreateBuffer(&bd, &sd, &buffer);

	if (numIndex) {
		hasIndex = true;
		D3D11_BUFFER_DESC ibd;
		ibd.ByteWidth = sizeof(UINT) * numIndex;
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.StructureByteStride = 0;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA isd;
		isd.pSysMem = indexData;

		framework->md3dDevice->CreateBuffer(&ibd, &isd, &ibuffer);

		drawCount = numIndex;
	}
}
void SEVAO::Bind()
{
	UINT stride = sizeof(VertexData);
	UINT offset = 0;
	framework->md3dContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
	if(hasIndex) framework->md3dContext->IASetIndexBuffer(ibuffer, DXGI_FORMAT_R32_UINT, 0);
}

void SEVAO::Draw()
{
	if (hasIndex)
		framework->md3dContext->DrawIndexed(drawCount, 0, 0);
	else
		framework->md3dContext->Draw(drawCount, 0);
}
