#pragma once

#include <DirectXMath.h>
#include <vector>
#include "quaternion.h"

class SESpring;

struct  MassVertex
{
	MassVertex();
	DirectX::FXMVECTOR ComputeForce();
	DirectX::XMFLOAT3 LocalPosition;
	SESpring *boundString;

	MassVertex(DirectX::FXMVECTOR pos):boundString(nullptr) {
		DirectX::XMStoreFloat3(&LocalPosition, pos);
	}
};

class SERigidBody
{
public:
	SERigidBody();
	~SERigidBody() {}

	void EnablePhysics() { physicsEnabled = true; }
	void LoadMassVertices(int numVertex, DirectX::FXMVECTOR * vertexList);
	DirectX::FXMVECTOR getCenterOfMass() { return DirectX::XMLoadFloat3(&centerOfMass); }
	MassVertex * getMassVertexByID(int id) { return &massVertices[id]; }
	DirectX::FXMVECTOR computeLinearDisplacement(float dt);
	DirectX::FXMVECTOR computeAngularDisplacement(float dt, SEQuaternion q);

	void calculateIneriaTensor();

	bool isEnabled() { return physicsEnabled; }

private:
	bool physicsEnabled;
	float mass;
	DirectX::XMFLOAT3 centerOfMass;
	DirectX::XMFLOAT3X3 Iobj;
	std::vector<MassVertex> massVertices;

	DirectX::XMFLOAT3 LinearMomentum;
	DirectX::XMFLOAT3 angularMomentum;
};