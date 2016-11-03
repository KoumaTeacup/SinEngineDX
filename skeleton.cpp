#include "skeleton.h"
#include "bone.h"
#include "d3dUtil.h"
#include "animation.h"
#include "framework.h"

SESkeleton::SESkeleton() {}

SESkeleton::~SESkeleton() {
	delete boneRoot;
}

void SESkeleton::Draw() {
	if (SE_Mode == SE_RENDER_PREPARE_DISPLAY_BONES) {
		SE_State.Set(SE_RENDER_DISPLAY_BONES);
		boneRoot->drawAll(nullptr);
		SE_State.Restore();
	}
}

void SESkeleton::Tick() {
	boneRoot->Tick();
}

void SESkeleton::Pause() {
	boneRoot->Pause();
}

void SESkeleton::Resume() {
	boneRoot->Resume();
}
