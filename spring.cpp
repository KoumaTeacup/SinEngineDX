#include "spring.h"
#include "framework.h"
#include "rigidbody.h"

using namespace DirectX;

void SESpring::Bind() {
	if(buffer) ReleaseCOM(buffer);

	D3D11_BUFFER_DESC bd;
	bd.ByteWidth = sizeof(XMFLOAT3) * 2;
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.StructureByteStride = 0;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA sd;
	XMFLOAT3 buf[2];
	XMVECTOR localVector;

	localVector = XMLoadFloat3(&anchor0->LocalPosition) * anchor0Object->getMovement().getWorldScale();
	localVector = anchor0Object->getMovement().getRotationRef().rotate(localVector);
	XMStoreFloat3(&buf[0], localVector + anchor0Object->computeRigidLocation());

	localVector = XMLoadFloat3(&anchor1->LocalPosition) * anchor1Object->getMovement().getWorldScale();
	localVector = anchor1Object->getMovement().getRotationRef().rotate(localVector);
	XMStoreFloat3(&buf[1], localVector + anchor1Object->computeRigidLocation());

	sd.pSysMem = &buf[0];

	if(buffer) ReleaseCOM(buffer);
	SEDevice->CreateBuffer(&bd, &sd, &buffer);
	UINT stride = sizeof(XMFLOAT3);
	UINT offset = 0;
	SEContext->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);
}

void SESpring::Tick(float dt) {
	SEAsset::Tick(dt);
}

void SESpring::Draw() {
	SEAsset::Draw();
	SE_Shader->UpdateConstantBuffer();

	Bind();

	D3D11_PRIMITIVE_TOPOLOGY pt;
	SEContext->IAGetPrimitiveTopology(&pt);
	SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	SEContext->Draw(2, 0);
	SEContext->IASetPrimitiveTopology(pt);
}

SESpring::SESpring() : buffer(nullptr), anchor0(nullptr), anchor1(nullptr), length(0.0f), SpringConstant(30.0f) {
	setType(SE_ASSET_STRING);
}

SESpring::~SESpring() {
	ReleaseCOM(buffer);
}

void SESpring::ConstructSpring(SEAsset * a0o, int MassVertexID0, SEAsset * a1o, int MassVertexID1) {
	anchor0Object = a0o; 
	anchor0 = a0o->getRigidbody().getMassVertexByID(MassVertexID0);
	anchor1Object = a1o;
	anchor1 = a1o->getRigidbody().getMassVertexByID(MassVertexID1);

	anchor0->boundString = this;
	anchor1->boundString = this;
	
	length = XMVector3Length(XMLoadFloat3(&anchor0->LocalPosition) * anchor0Object->getMovement().getWorldScale()
		- XMLoadFloat3(&anchor1->LocalPosition) * anchor1Object->getMovement().getWorldScale()).m128_f32[0];
}

DirectX::FXMVECTOR SESpring::computeForce(MassVertex *end) const {
	XMVECTOR v0 = XMLoadFloat3(&anchor0->LocalPosition) * anchor0Object->getMovement().getWorldScale();
	XMVECTOR v1 = XMLoadFloat3(&anchor1->LocalPosition) * anchor1Object->getMovement().getWorldScale();
	v0 = anchor0Object->getMovement().getRotationRef().rotate(v0);
	v1 = anchor1Object->getMovement().getRotationRef().rotate(v1);
	v0 += anchor0Object->computeRigidLocation();
	v1 += anchor1Object->computeRigidLocation();
	
	XMVECTOR dir = v0 - v1;

	if(end == anchor0) {
		return -dir * SpringConstant;
	}
	else return dir*SpringConstant;
}
