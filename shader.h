#pragma once

#include "d3dUtil.h"

enum InputLayoutID
{
	SE_INPUT_LAYOUT_UNDEFINED,
	SE_INPUT_LAYOUT_VERTEX_DATA,
	SE_INPUT_LAYOUT_CURVE_DATA
};

class D3DApp;

struct VSConstantBufferLayout
{
	DirectX::XMFLOAT4X4 mvp;
};

struct PSConstantBufferLayout
{
	DirectX::XMFLOAT4 dummy;
};

class SEShader
{
public:
	SEShader() : 
		vertexShader(nullptr),
		pixelShader(nullptr), 
		inputLayout(nullptr)
	{}
	~SEShader();

	void Init(const char* _filename, InputLayoutID id);
	void Bind();
	void UpdateConstantBuffer();
	VSConstantBufferLayout &GetVSConstantData() { return VSLocalConstantCopy; }
	PSConstantBufferLayout &GetPSConstantData() { return PSLocalConstantCopy; }

private:
	ID3D11PixelShader *pixelShader;
	ID3D11VertexShader *vertexShader;
	ID3D11InputLayout *inputLayout;
	VSConstantBufferLayout VSLocalConstantCopy;
	PSConstantBufferLayout PSLocalConstantCopy;
	ID3D11Buffer *VSConstantBuffer;
	ID3D11Buffer *PSConstantBuffer;
};