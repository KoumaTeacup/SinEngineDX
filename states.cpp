#include "states.h"
#include "d3dApp.h"

#include "framework.h"

void SERenderStates::Set(SERenderMode mode = SE_RENDER_DEFAULT) {
	switch (mode) {
	case SE_RENDER_DEFAULT:
		framework->md3dContext->RSSetState(standard);
		break;
	case SE_RENDER_WIREFRAME:
		framework->md3dContext->RSSetState(wireframe);
		break;
	case SE_RENDER_DISABLE_FACE_CULLING:
		framework->md3dContext->RSSetState(noFaceCulling);
		break;
	default:
		framework->md3dContext->RSSetState(standard);
		break;
	}
}

void SERenderStates::Init() {
	D3D11_RASTERIZER_DESC rd_standard;
	ZeroMemory(&rd_standard, sizeof(D3D11_RASTERIZER_DESC));
	rd_standard.CullMode = D3D11_CULL_BACK;
	rd_standard.FillMode = D3D11_FILL_SOLID;
	rd_standard.FrontCounterClockwise = false;
	rd_standard.DepthClipEnable = true;

	framework->md3dDevice->CreateRasterizerState(&rd_standard, &standard);

	D3D11_RASTERIZER_DESC rd_wireframe;
	ZeroMemory(&rd_wireframe, sizeof(D3D11_RASTERIZER_DESC));
	rd_wireframe.CullMode = D3D11_CULL_NONE;
	rd_wireframe.FillMode = D3D11_FILL_WIREFRAME;
	rd_wireframe.FrontCounterClockwise = false;
	rd_wireframe.DepthClipEnable = true;

	framework->md3dDevice->CreateRasterizerState(&rd_wireframe, &wireframe);

	D3D11_RASTERIZER_DESC rd_noFaceCulling;
	ZeroMemory(&rd_noFaceCulling, sizeof(D3D11_RASTERIZER_DESC));
	rd_noFaceCulling.CullMode = D3D11_CULL_NONE;
	rd_noFaceCulling.FillMode = D3D11_FILL_SOLID;
	rd_noFaceCulling.FrontCounterClockwise = false;
	rd_noFaceCulling.DepthClipEnable = true;

	framework->md3dDevice->CreateRasterizerState(&rd_noFaceCulling, &noFaceCulling);

	Set();
}
