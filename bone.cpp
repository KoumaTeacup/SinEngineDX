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

	// Convert to matrix to shader and scale to defined joint radius
	XMMATRIX M = transFinal.toMatrixWithScale(XMVectorSet(SE_BONE_RADIUS, SE_BONE_RADIUS, SE_BONE_RADIUS, 0.0f));
	XMFLOAT4X4 originalM = SE_Shader->GetVSConstantData().mvp;
	XMStoreFloat4x4(&SE_Shader->GetVSConstantData().mvp, M * XMLoadFloat4x4(&originalM));
	SE_Shader->UpdateConstantBuffer();

	// Draw Mesh
	jointMesh.Render();

	if (parent) {
		XMVECTOR boneVector = transFinal.getV() - parent->transFinal.getV();

		XMFLOAT3 f3;
		XMStoreFloat3(&f3, XMVector3LengthEst(boneVector));

		// Bone mesh requires non-uniform scaling so we use matrix instead of vqs
		SEQuaternion boneOrient = SEQuaternion::rotationFromUnitX(boneVector);

		XMStoreFloat4x4(
			&SE_Shader->GetVSConstantData().mvp,
				XMMatrixScaling(f3.x - SE_BONE_RADIUS * 2.0f, SE_BONE_RADIUS, SE_BONE_RADIUS)
				* XMMatrixTranslation(SE_BONE_RADIUS, 0.0f, 0.0f)
				* XMMatrixRotationQuaternion(boneOrient.get())
				* XMMatrixTranslationFromVector(parent->transFinal.getV())
				* XMLoadFloat4x4(&originalM)
		);

		SE_Shader->UpdateConstantBuffer();
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		boneMesh.Render();
		SEContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);
	}

	SE_Shader->GetVSConstantData().mvp = originalM;
	
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

void SEBone::Tick(float dt, SEBone * parent) {
	for (auto i : animations) i->Tick();

	// Retrieve parent transformation
	SEVQS parentTransform = parent ? parent->transFinal : SEVQS();

	// Assuming 1 animation
	transAnim = animations[0]->getTransformiSLerped();

	// Store local transformation
	transFinal = transAnim * transOrient* parentTransform;

	for (auto i : children)i->Tick(dt, this);
}

void SEBone::Resume() {
	for (auto i : animations) i->Restart();
	for (auto i : children) i->Resume();
}

void SEBone::Pause() {
	for (auto i : animations) i->Pause();
	for (auto i : children) i->Pause();
}

void SEBone::setAnimationTimeScale(int animationID, float scale) {
	animations[animationID]->setTimeScale(scale); 
	for(auto i : children) i->setAnimationTimeScale(animationID, scale);
}

void SEBone::clearIKRefinement() {
	ikRefinement.identify();
	for(auto i : children) i->clearIKRefinement();
}

XMVECTOR SEBone::resolveIK(SEBone *parent, XMVECTOR effector) {

	XMVECTOR local = transFinal.getV();

	if(children.size() == 0) return transFinal.getV();
	XMVECTOR endPos = children.front()->resolveIK(this, effector);
	XMVECTOR vc = endPos - local;
	XMVECTOR vd = effector - local;
	XMVECTOR dotProduct = XMVector3Dot(vc, vd);
	XMVECTOR crossProduct = XMVector3Normalize(XMVector3Cross(vc, vd));
	XMVECTOR lenc = XMVector3Length(vc);
	XMVECTOR lend = XMVector3Length(vd);
	float theta = dotProduct.m128_f32[0] / lenc.m128_f32[0] / lend.m128_f32[0];
	if(theta > 1.0f) theta -= 0.0001f;
	if(theta < -1.0f) theta += -0.0001f;

	float alpha = acosf(theta);
	crossProduct *= sinf(alpha / 2.0f);
	SEQuaternion newRefinement(cosf(alpha / 2.0f), crossProduct);
	SEQuaternion concatRefinement =  newRefinement * ikRefinement;
	ikRefinement = newRefinement * ikRefinement;
	return local + newRefinement.rotate(vc);
}

void SEBone::resolveTransformation(SEBone *parent) {
	if(parent) {
		SEVQS parentTransform = parent ? parent->transFinal : SEVQS();
		transFinal = transAnim;
		transFinal *= transOrient;
		transFinal *= parentTransform;
	}
	else {
		transFinal.setQ(transAnim.getQ());
		transFinal *= transOrient;
	}
	transFinal.setQ(ikRefinement * transFinal.getQ());
	for(auto i : children) i->resolveTransformation(this);

}
