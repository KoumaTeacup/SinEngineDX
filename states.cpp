#include "states.h"
#include "framework.h"

SERenderStates::SERenderStates() :
	standard(nullptr),
	wireframe(nullptr),
	noFaceCulling(nullptr),
	saved(SE_RENDER_DEFAULT) {
}


void SERenderStates::Set(SERenderMode mode = SE_RENDER_DEFAULT) {
	switch (mode) {
	case SE_RENDER_DEFAULT:
		saved = SE_Mode;
		SE_Mode = mode;
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		SEContext->RSSetState(standard);
		break;
	case SE_RENDER_WIREFRAME:
		saved = SE_Mode;
		SE_Mode = mode;
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		SEContext->RSSetState(wireframe);
		break;
	case SE_RENDER_DISABLE_FACE_CULLING:
		saved = SE_Mode;
		SE_Mode = mode;
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		SEContext->RSSetState(noFaceCulling);
		break;
	case SE_RENDER_DISPLAY_BONES:
		saved = SE_Mode;
		SE_Mode = mode;
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
		SEContext->RSSetState(xrayBones);
		break;
	case SE_RENDER_PREPARE_DISPLAY_BONES:
		saved = SE_Mode;
		SE_Mode = mode;
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;
	default:
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		SEContext->RSSetState(standard);
		break;
	}
}

void SERenderStates::Restore() {
	Set(saved);
}

void SERenderStates::Init() {
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));

	rd.CullMode = D3D11_CULL_BACK;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;
	SEDevice->CreateRasterizerState(&rd, &standard);

	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;
	SEDevice->CreateRasterizerState(&rd, &wireframe);

	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_SOLID;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = true;
	SEDevice->CreateRasterizerState(&rd, &noFaceCulling);

	rd.CullMode = D3D11_CULL_NONE;
	rd.FillMode = D3D11_FILL_WIREFRAME;
	rd.FrontCounterClockwise = false;
	rd.DepthClipEnable = false;
	SEDevice->CreateRasterizerState(&rd, &xrayBones);

	Set();
}
