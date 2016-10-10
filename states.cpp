#include "states.h"
#include "d3dApp.h"

void SERenderStates::Set() {
	framework->md3dContext->RSSetState(state);
}

void SERenderStates::Init() {
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));
	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;//D3D11_FILL_WIREFRAME D3D11_FILL_SOLID
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;

	framework->md3dDevice->CreateRasterizerState(&rd, &state);
}
