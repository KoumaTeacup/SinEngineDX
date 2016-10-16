#include "shader.h"
#include "d3dUtil.h"
#include "d3dApp.h"
#include <fstream>

SEShader::~SEShader() {
	if (pixelShader) ReleaseCOM(pixelShader);
	if (vertexShader) ReleaseCOM(vertexShader);
	if (inputLayout) ReleaseCOM(inputLayout);
	if (VSConstantBuffer) ReleaseCOM(VSConstantBuffer);
	if (PSConstantBuffer) ReleaseCOM(PSConstantBuffer);

}

void SEShader::Init(const char * _filename) {
	std::string filename(_filename);
	std::ifstream ifs;

	ifs.open("ShaderOut/" + filename + "VS.cso", std::ios::binary);
	if (ifs) {

		// Read vertex shader source
		ifs.seekg(0, ifs.end);	
		UINT length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		char *content = new char[length];
		ifs.read(content, length);
		ifs.close();

		// Create vertex shader
		SEDevice->CreateVertexShader(content, length, nullptr, &vertexShader);

		// Define & Create vertex shader input
		D3D11_INPUT_ELEMENT_DESC ied[vertexDataInputCount] = {
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexData, VertexData::position), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexData, VertexData::normal), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(VertexData, VertexData::texCoord), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexData, VertexData::tangent), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, offsetof(VertexData, VertexData::color), D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"USE_VERTEX_COLOR", 0, DXGI_FORMAT_R32_UINT, 0, offsetof(VertexData, VertexData::useVertexColor), D3D11_INPUT_PER_VERTEX_DATA, 0}
		};

		SEDevice->CreateInputLayout(ied, ARRAYSIZE(ied), content, length, &inputLayout);

		// Define & Create vertex shader constant buffer
		D3D11_BUFFER_DESC cbd = {0};
		cbd.ByteWidth = sizeof(VSConstantBufferLayout);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;
			
		SEDevice->CreateBuffer(&cbd, nullptr, &VSConstantBuffer);

	}

	ifs.open("ShaderOut/" + filename + "PS.cso", std::ios::binary);
	if (ifs) {
		// Read vertex shader source
		ifs.seekg(0, ifs.end);
		UINT length = ifs.tellg();
		ifs.seekg(0, ifs.beg);
		char *content = new char[length];
		ifs.read(content, length);
		ifs.close();

		// Create pixel shader
		SEDevice->CreatePixelShader(content, length, nullptr, &pixelShader);

		// Define & Create pixel shader constant buffer
		D3D11_BUFFER_DESC cbd = {0};
		cbd.ByteWidth = sizeof(PSConstantBufferLayout);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;

		SEDevice->CreateBuffer(&cbd, nullptr, &PSConstantBuffer);

	}
}

void SEShader::Bind() {
	// Bind all shaders for this pass
	SEContext->VSSetShader(vertexShader, nullptr, 0);
	SEContext->PSSetShader(pixelShader, nullptr, 0);
	SEContext->IASetInputLayout(inputLayout);
}

void SEShader::Draw() {

	// Update constant buffer
	D3D11_MAPPED_SUBRESOURCE mappedSubresource;
	ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	SEContext->Map(VSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &VSLocalConstantCopy.mvp, sizeof(VSConstantBufferLayout));
	SEContext->Unmap(VSConstantBuffer, 0);

	/*ZeroMemory(&mappedSubresource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	SEContext->Map(PSConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy(mappedSubresource.pData, &PSLocalConstantCopy, sizeof(PSConstantBufferLayout));
	SEContext->Unmap(PSConstantBuffer, 0);*/

	// Bind all shader constant for this pass
	SEContext->VSSetConstantBuffers(0, 1, &VSConstantBuffer);
	SEContext->PSSetConstantBuffers(0, 1, &PSConstantBuffer);
}