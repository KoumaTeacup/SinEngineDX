#include "skeleton.h"
#include "bone.h"
#include "d3dUtil.h"
#include "animation.h"
#include "framework.h"

SESkeleton::SESkeleton() :boneRoot(nullptr), rootTrans(0.0f, 0.0f, 0.0f) {
	setType(SE_ASSET_SKELETON);
}

SESkeleton::~SESkeleton() {
	delete boneRoot;
}

void SESkeleton::Draw() {
	SEAsset::Draw();
	if (SE_Mode == SE_RENDER_PREPARE_DISPLAY_BONES) {
		SE_State.Set(SE_RENDER_DISPLAY_BONES);
		boneRoot->drawAll(nullptr);
		SE_State.Restore();
	}
}

void SESkeleton::Tick(float dt) {
	SEAsset::Tick(dt);
	boneRoot->Tick(dt);
}

void SESkeleton::Pause() {
	boneRoot->Pause();
}

void SESkeleton::Resume() {
	boneRoot->Resume();
}

void SESkeleton::resolveIK(DirectX::XMVECTOR effector, float dt) {

	XMVECTOR oldEnd, newEnd = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//boneRoot->clearIKRefinement();

	boneRoot->setFinalTranslation(XMLoadFloat3(&rootTrans));
	boneRoot->resolveTransformation(nullptr);
	
	XMVECTOR vec3;
	do {
		oldEnd = newEnd;
		newEnd = boneRoot->resolveIK(nullptr, effector);
		boneRoot->resolveTransformation(nullptr);
		
		vec3 = XMVector3Length(newEnd - effector);
		if(vec3.m128_f32[0] < 1.0f) break;
		vec3 = XMVector3Length(oldEnd - newEnd);
	} while(vec3.m128_f32[0] > 0.1f);

	if(XMVector3Length(newEnd - effector).m128_f32[0] > 3.0f) {
		if(!getMovement().tableReady) getMovement().constructTable();
		XMStoreFloat3(&rootTrans, getMovement().getTickTranslation(dt));
		rootTrans.y = 0.0f;
	}
}
