#include "movement.h"

#include "framework.h"
#include "spline.h"
#include "quaternion.h"
#include "skeleton.h"
#include "bone.h"
#include <map>

#define ERROR_THRESHOLD 1.f
#define LENGTH_THRESHOLD 5.f

using namespace DirectX;

SEMovement::SEMovement() :
	followCurve(false),
	tableReady(false),
	pathCurve(nullptr),
	worldTranslate(0.0f, 0.0f, 0.0f),
	worldScale(1.0f, 1.0f, 1.0f),
	worldRotation(),
	accumulatedTime(0.0f),
	curveLength(0.0f){}

void SEMovement::AssignCurve(SESpline * _pathCurve) { 
	pathCurve = _pathCurve; 
	constructTable();
}

void SEMovement::EnablePath() {
	followCurve = true;
	accumulatedTime = 0.0f;
}

void SEMovement::AddWorldTranslate(FXMVECTOR trans) {
	XMVECTOR vec = XMLoadFloat3(&worldTranslate);
	vec += trans;
	XMStoreFloat3(&worldTranslate, vec);
	worldTranslate.x = worldTranslate.x > 250.0f ? 250.0f : worldTranslate.x;
	worldTranslate.x = worldTranslate.x < -250.0f ? -250.0f : worldTranslate.x;
	worldTranslate.y = worldTranslate.y > 200.0f ? 200.0f : worldTranslate.y;
	worldTranslate.y = worldTranslate.y < 0.0 ? 0.0f : worldTranslate.y;
	worldTranslate.z = worldTranslate.z > 250.0f ? 250.0f : worldTranslate.z;
	worldTranslate.z = worldTranslate.z < -250.0f ? -250.0f : worldTranslate.z;
}

void SEMovement::SetScale(DirectX::FXMVECTOR scale) {
	XMStoreFloat3(&worldScale, scale);
}

DirectX::FXMVECTOR SEMovement::getWorldTranslate() {
	return XMLoadFloat3(&worldTranslate);
}

DirectX::FXMVECTOR SEMovement::getWorldScale() {
	return XMLoadFloat3(&worldScale);
}

void SEMovement::computePhysicsRotation(DirectX::FXMVECTOR omega) {
	SEQuaternion R = XMQuaternionRotationAxis(omega, XMVector3Length(omega).m128_f32[0]);
	worldRotation = R.normalize() * worldRotation;
}

void SEMovement::Tick(float dt, SEAsset *owner) {
	if(!followCurve) return;

	float s, v;
	float timeScale = 0.02f;
	dt *= timeScale;
	accumulatedTime = accumulatedTime + dt;
	accumulatedTime -= floorf(accumulatedTime);


	if(accumulatedTime < 0.3f) {
		v = sinf((accumulatedTime / 0.3f - 0.5f) * XM_PI) / 2.0f + 0.5f;
		s = -cosf((accumulatedTime/0.3f - 0.5f) * XM_PI) / 2.0f / XM_PI * 0.3f + 0.5f * accumulatedTime;
	}
	else if(accumulatedTime > 0.7f) {
		v = -sinf(((accumulatedTime - 0.7f) / 0.3f - 0.5f) * XM_PI) / 2.0f + 0.5f;
		s = cosf(((accumulatedTime - 0.7f) / 0.3f - 0.5f) * XM_PI) / 2.0f / XM_PI * 0.3f + 0.5f * (accumulatedTime - 0.7f);
		s += 0.55f;
	}
	else {
		s = 0.15f + (accumulatedTime - 0.3f);
		v = 1.0f;
	}

	s /= 0.7f;

	XMStoreFloat3(&worldTranslate, arcLengthReverse(s));

	float ds = 0.02f;
	XMVECTOR faceDir = (arcLengthReverse(s + ds) - arcLengthReverse(s));

	worldRotation = SEQuaternion::rotationFromUnitX(faceDir);

	if(owner->getType() == SE_ASSET_SKELETON) {
		static_cast<SESkeleton*>(owner)->getRootBone()->setAnimationTimeScale(0, v);
	}
}

void SEMovement::Draw() {
	XMStoreFloat4x4(&SE_Shader->GetVSConstantData().mvp,
		XMMatrixScaling(worldScale.x, worldScale.y, worldScale.z)*
		XMMatrixRotationQuaternion(worldRotation.get()) *
		XMMatrixTranslation(worldTranslate.x, worldTranslate.y, worldTranslate.z) *
		SE_VP);
}

DirectX::XMVECTOR SEMovement::getTickTranslation(float dt) {
	float s, v;
	float timeScale = 0.2f;
	dt *= timeScale;
	accumulatedTime = accumulatedTime + dt;
	if(accumulatedTime > 1.0f) accumulatedTime = 1.0f;


	if(accumulatedTime < 0.3f) {
		v = sinf((accumulatedTime / 0.3f - 0.5f) * XM_PI) / 2.0f + 0.5f;
		s = -cosf((accumulatedTime / 0.3f - 0.5f) * XM_PI) / 2.0f / XM_PI * 0.3f + 0.5f * accumulatedTime;
	}
	else if(accumulatedTime > 0.7f) {
		v = -sinf(((accumulatedTime - 0.7f) / 0.3f - 0.5f) * XM_PI) / 2.0f + 0.5f;
		s = cosf(((accumulatedTime - 0.7f) / 0.3f - 0.5f) * XM_PI) / 2.0f / XM_PI * 0.3f + 0.5f * (accumulatedTime - 0.7f);
		s += 0.55f;
	}
	else {
		s = 0.15f + (accumulatedTime - 0.3f);
		v = 1.0f;
	}

	s /= 0.7f;

	if(s > 1.0f) s -= 0.001f;
	if(s < 0.0f) s += 0.001f;
	return arcLengthReverse(s);
}

void SEMovement::constructTable() {
	
	arcLengthTable.clear();
	tableReady = true;
	if(!pathCurve || pathCurve->isEmpty()) {
		return;
	}

	XMFLOAT3 f3;
	std::map<float, float> entryList;
	entryList[0.0f] = 0.0f;
	XMStoreFloat3(&f3, XMVector3Length(pathCurve->computeCurve(1.0f) - pathCurve->computeCurve(0.0f)));
	entryList[1.0f] = f3.x;

	bool bFlag = true;
	while(bFlag) {
		bFlag = false;
		for(auto i = entryList.begin(); i != entryList.end(); i++) {
			auto j = i;
			j++;
			if(j != entryList.end()) {
				float um = (i->first + j->first) / 2.f;
				XMVECTOR posM = pathCurve->computeCurve(um);
				XMVECTOR dis = XMVector3Length(pathCurve->computeCurve(j->first) - pathCurve->computeCurve(i->first));
				XMVECTOR dis0 = XMVector3Length(posM - pathCurve->computeCurve(i->first));
				XMVECTOR dis1 = XMVector3Length(pathCurve->computeCurve(j->first) - posM);
				XMFLOAT3 error;
				XMStoreFloat3(&error, dis0 + dis1);
				XMStoreFloat3(&f3, dis);
				if(error.x - f3.x> ERROR_THRESHOLD || error.x > LENGTH_THRESHOLD) {
					XMStoreFloat3(&f3, dis0);
					entryList[um] = i->second + f3.x;
					XMStoreFloat3(&f3, dis1);
					j->second = entryList[um] + f3.x;
					bFlag = true;
				}
				else {
					j->second = i->second + error.x;
				}
			}
		}
	}

	for(auto i : entryList) arcLengthTable.push_back(arcLengthTableEntry(i.first, i.second));
	curveLength = arcLengthTable.back().arcLength;
	for(auto &i : arcLengthTable) i.arcLength /= curveLength;
}

DirectX::XMVECTOR SEMovement::arcLengthReverse(float s) {
	if(arcLengthTable.size() < 2) return XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	return arcLengthReverse(s, 0, arcLengthTable.size() - 1);
}

DirectX::XMVECTOR SEMovement::arcLengthReverse(float s, int begin, int end) {
	if(end - begin == 1) {
		float t = (s - arcLengthTable[begin].arcLength) / (arcLengthTable[end].arcLength - arcLengthTable[begin].arcLength); 
		float u = (1 - t)*arcLengthTable[begin].u + t*arcLengthTable[end].u;
		return pathCurve->computeCurve(u);
	}

	if(s > arcLengthTable[(begin + end) / 2].arcLength)
		return arcLengthReverse(s, (begin + end) / 2, end);
	else
		return arcLengthReverse(s, begin, (begin + end) / 2);
}
