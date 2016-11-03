#include "bone.h"
#include "animation.h"
#include "mesh.h"
#include "framework.h"
#include "quaternion.h"

#include "meshPresets.h"

#define SE_BONE_RADIUS 0.5f

using namespace SE_Internal_Mesh;

SEMesh SEBone::boneMesh = SEMesh();//boneMeshVCount, boneMeshV, boneMeshICount, boneMeshI);
SEMesh SEBone::jointMesh = SEMesh();// jointMeshVCount, jointMeshV, jointMeshICount, jointMeshI);

SEBone::~SEBone() {
	for (auto i : children) {
		delete i;
	}

	for (auto i : animations) {
		delete i;
	}
}

void SEBone::loadBone(std::string _name, DirectX::XMFLOAT3 trans, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scl, const SEQuaternion &_orient) {
	name = _name;
	localTranslation = trans;
	localRotation = rot;
	localScale = scl;
	orient = _orient;

	transLocal = SEVQS(XMLoadFloat3(&localTranslation), SEQuaternion::fromEuler(XMLoadFloat3(&localRotation)));
	transOrient = SEVQS(orient);
	transLocal *= transOrient;
}

void SEBone::addChild(SEBone * child) {
	children.push_back(child);
}

void SEBone::drawAll(SEBone *parent) {

	// Retrieve parent transformation
	SEVQS parentTransform = parent ? parent->transFinal : SEVQS();

	// Assuming 1 animation
	transAnim = animations[0]->getTransformiSLerped();

	// Store local transformation
	transFinal = transAnim * transOrient* parentTransform;

	// Convert to matrix to shader and scale to defined joint radius
	XMMATRIX M = transFinal.toMatrixWithScale(XMVectorSet(SE_BONE_RADIUS, SE_BONE_RADIUS, SE_BONE_RADIUS, 0.0f));
	XMStoreFloat4x4(&SE_Shader.GetVSConstantData().mvp, XMMatrixTranspose(M * SE_VP));
	SE_Shader.Draw();

	// Draw Mesh
	jointMesh.Bind();
	jointMesh.Draw();

	if (parent) {
		XMVECTOR boneVector = transFinal.getV() - parent->transFinal.getV();

		XMFLOAT3 f3;
		XMStoreFloat3(&f3, XMVector3LengthEst(boneVector));

		// Bone mesh requires non-uniform scaling so we use matrix instead of vqs
		SEQuaternion boneOrient = SEQuaternion::rotationFromUnitX(boneVector);

		XMStoreFloat4x4(
			&SE_Shader.GetVSConstantData().mvp,
			XMMatrixTranspose(
				XMMatrixScaling(f3.x - SE_BONE_RADIUS * 2.0f, SE_BONE_RADIUS, SE_BONE_RADIUS)
				* XMMatrixTranslation(SE_BONE_RADIUS, 0.0f, 0.0f)
				* XMMatrixRotationQuaternion(boneOrient.get())
				* XMMatrixTranslationFromVector(parent->transFinal.getV())
				* SE_VP
			)
		);

		SE_Shader.Draw();
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		boneMesh.Bind();
		boneMesh.Draw();
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}
	
	for (auto i : children) {
		i->drawAll(this);
	}
}

void SEBone::InitBoneMesh() {
	boneMesh.Load(boneMeshVCount, boneMeshV, boneMeshICount, boneMeshI);
	jointMesh.Load(jointMeshVCount, jointMeshV, jointMeshICount, jointMeshI);
}

void SEBone::Reset() {
	for (auto i : animations) i->Reset();
	for (auto i : children) i->Reset();
}

void SEBone::Tick() {
	for (auto i : animations) i->Tick();
	for (auto i : children)i->Tick();
}

void SEBone::Resume() {
	for (auto i : animations) i->Restart();
	for (auto i : children) i->Resume();
}

void SEBone::Pause() {
	for (auto i : animations) i->Pause();
	for (auto i : children) i->Pause();
}
