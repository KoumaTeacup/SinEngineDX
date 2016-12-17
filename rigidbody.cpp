#include "rigidbody.h"
#include "spring.h"

using namespace DirectX;

SERigidBody::SERigidBody() :
	mass(100.0f),
	centerOfMass(0.0f, 0.0f, 0.0f),
	physicsEnabled(false),
	LinearMomentum(0.0f, 0.0f, 0.0f),
	angularMomentum(0.0f, 0.0f, 0.0f) {}

void SERigidBody::LoadMassVertices(int numVertex, DirectX::FXMVECTOR * vertexList) {
	XMVECTOR sum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	int i = 0;
	for(; i < numVertex; i++) {
		massVertices.push_back(MassVertex(vertexList[i]));
		sum += vertexList[i];
	}

	sum /= (float)i;

	for(auto q : massVertices) {
		XMVECTOR v3 = XMLoadFloat3(&q.LocalPosition);
		v3 -= sum;
		XMStoreFloat3(&q.LocalPosition, v3);
	}

	XMStoreFloat3(&centerOfMass, sum);

	calculateIneriaTensor();
}

FXMVECTOR SERigidBody::computeLinearDisplacement(float dt) {
	if(!physicsEnabled) return XMVECTOR();

	XMVECTOR netForce = XMVECTOR();
	for(auto &i : massVertices) netForce += i.ComputeForce();

	netForce += mass * XMVectorSet(0.0f, -9.8f, 0.0f, 0.0f);
	XMVECTOR newMomentum = XMLoadFloat3(&LinearMomentum) + netForce*dt;
	XMStoreFloat3(&LinearMomentum, XMLoadFloat3(&LinearMomentum) * 0.99f + netForce*dt);
	return newMomentum/ mass * dt;
}

DirectX::FXMVECTOR SERigidBody::computeAngularDisplacement(float dt, SEQuaternion q) {
	if(!physicsEnabled) return XMVECTOR();

	XMVECTOR netTorque = XMVECTOR();
	//for(auto &i : massVertices) netTorque += XMVector3Cross(XMLoadFloat3(&i.LocalPosition), i.ComputeForce());
	for(auto &i : massVertices) {
		XMVECTOR force = i.ComputeForce();
		XMVECTOR t = XMVector3Cross(q.rotate(XMLoadFloat3(&i.LocalPosition)), force);
		netTorque += t;
	}

	XMStoreFloat3(&angularMomentum, XMLoadFloat3(&angularMomentum) * 0.99f + netTorque * dt);

	XMMATRIX MatMomentum = XMMatrixSet(
		angularMomentum.x, 0.0f, 0.0f, 0.0f,
		angularMomentum.y, 0.0f, 0.0f, 0.0f,
		angularMomentum.z, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f
		);

	XMMATRIX matI = XMLoadFloat3x3(&Iobj);
	XMMATRIX matR = q.toMatrix();
	matI = matR* matI * XMMatrixTranspose(matR);

	XMFLOAT3X3 matVelocity;
	XMStoreFloat3x3(&matVelocity, matI * MatMomentum);

	XMVECTOR angularVelocity = XMVectorSet(matVelocity._11, matVelocity._21, matVelocity._31, 0.0f) * dt;
	return angularVelocity;
}

void SERigidBody::calculateIneriaTensor() {
	float Ixx = 0.0f, Iyy = 0.0f, Izz = 0.0f, Ixy = 0.0f, Ixz = 0.0f, Iyz = 0.0f;
	XMMATRIX matI;
	for(auto i : massVertices) {
		Ixx += i.LocalPosition.y * i.LocalPosition.y + i.LocalPosition.z + i.LocalPosition.z;
		Iyy += i.LocalPosition.x * i.LocalPosition.x + i.LocalPosition.z + i.LocalPosition.z;
		Izz += i.LocalPosition.x * i.LocalPosition.x + i.LocalPosition.y + i.LocalPosition.y;
		Ixy += i.LocalPosition.x * i.LocalPosition.y;
		Ixz += i.LocalPosition.x * i.LocalPosition.z;
		Iyz += i.LocalPosition.y * i.LocalPosition.z;
	}
	matI = XMMatrixSet(Ixx, Ixy, Ixz, 0.0f, Ixy, Iyy, Iyz, 0.0f, Ixz, Iyz, Izz, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	matI *= mass;
	matI = XMMatrixInverse(&XMMatrixDeterminant(matI), matI);
	XMStoreFloat3x3(&Iobj, matI);
}

MassVertex::MassVertex():boundString(nullptr), LocalPosition(0.0f, 0.0f, 0.0f) {}

DirectX::FXMVECTOR MassVertex::ComputeForce() {
	if (boundString) return boundString->computeForce(this);
	else return XMVECTOR();
}
