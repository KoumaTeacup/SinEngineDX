#include "animation.h"
#include "bone.h"

using namespace DirectX;

void SEAnimation::Init() {
	if (keyFrames.size() < 1) return;

	auto it = keyFrames.begin();
	auto temp = keyFrames.end();

	for (; it != keyFrames.end(); ++it) {
		it->second.last = temp;
		temp = it;
	}

	temp = keyFrames.end();
	do {
		--it;
		it->second.next = temp;
		temp = it;
	} while (it != keyFrames.begin());
	
	for (auto &i : keyFrames) i.second.constructVQS(i.first, keyFrames.end());
}

void SEAnimation::Reset() {
	timer.Reset();

	if (keyFrames.size() < 1) return;
	currentKey = keyFrames.begin();
	stepTime = 0.0f;
	currentStep = currentKey->second.getVQS();

	auto next = currentKey;
	if (++next == keyFrames.end()) {
		// only one key frame, no interpolation needed
		XMStoreFloat3(&stepV, XMVectorZero());
		stepQ = SEQuaternion();
		XMStoreFloat3(&stepS, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else {
		// recalculate frame steps
		SEVQS vqs0 = currentKey->second.getVQS();
		SEVQS vqs1 = next->second.getVQS();

		XMStoreFloat3(&stepV, (vqs1.getV() - vqs0.getV()) / SE_NUM_INTERPOLATION_STEP);
		stepQ = SEQuaternion::incrementalStep(vqs0.getQ(), vqs1.getQ(), SE_NUM_INTERPOLATION_STEP);
		XMStoreFloat3(&stepS, XMVectorSet(powf(vqs1.getS() / vqs0.getS(), 1.0f / SE_NUM_INTERPOLATION_STEP), 0.0f, 0.0f, 0.0f));

		currentStep = currentKey->second.getVQS();
	}
}

void SEAnimation::Pause() {
	timer.Stop();
}

void SEAnimation::Restart() {
	//timer.Start();
}

void SEAnimation::Tick() {
	timer.Tick();

	if (keyFrames.size() < 1) return;
	auto next = currentKey;
	if (++next == keyFrames.end()) Reset();
	else if (timer.TotalTime() * SE_ANIMATION_FPS > next->first) {
		currentKey = next;
		stepTime = 0.0f;
		if (++next == keyFrames.end()) {
			// only one key frame, no interpolation needed
			XMStoreFloat3(&stepV, XMVectorZero());
			stepQ = SEQuaternion();
			XMStoreFloat3(&stepS, XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else {
			// recalculate frame steps
			SEVQS vqs0 = currentKey->second.getVQS();
			SEVQS vqs1 = next->second.getVQS();

			XMStoreFloat3(&stepV, (vqs1.getV() - vqs0.getV()) / SE_NUM_INTERPOLATION_STEP);
			stepQ = SEQuaternion::incrementalStep(vqs0.getQ(), vqs1.getQ(), SE_NUM_INTERPOLATION_STEP);
			XMStoreFloat3(&stepS, XMVectorSet(powf(vqs1.getS() / vqs0.getS(), 1.0f / SE_NUM_INTERPOLATION_STEP), 0.0f, 0.0f, 0.0f));

			currentStep = currentKey->second.getVQS();
		}
	}
}

void SEAnimation::insert(FbxAnimCurve * curve, elementType eType, FbxTime::EMode timeMode) {
	if (curve) {
		int keyCount = curve->KeyGetCount();
		for (int iKey = 0; iKey < keyCount; iKey++) {
			SEKeyFrame keyFrame(eType, bone);
			int frameCount;
			frameCount = int(curve->KeyGetTime(iKey).GetFrameCount(timeMode));
			keyFrame.setValue(eType, curve->KeyGetValue(iKey));
			FbxAnimCurveDef::EInterpolationType lerpType = curve->KeyGetInterpolation(iKey);
			switch (lerpType) {
			case fbxsdk::FbxAnimCurveDef::eInterpolationCubic:
				keyFrame.setTangent(eType, SE_TANGENT_CUBIC);
				break;
			case fbxsdk::FbxAnimCurveDef::eInterpolationLinear:
				keyFrame.setTangent(eType, SE_TANGENT_LINEAR);
				break;
			default:
				keyFrame.setTangent(eType, SE_TANGENT_UNKNOWN);
				break;
			}

			keyFrames[frameCount] = keyFrame;
		}
	}
}

SEVQS SEAnimation::getTransformLerped() {
	if (keyFrames.size() > 0) {
		auto next = currentKey;
		if (++next != keyFrames.end()) {
			float currentFrame = timer.TotalTime() * SE_ANIMATION_FPS;
			float t = (currentFrame - currentKey->first) / (next->first - currentKey->first);
			if (t > 1.0f) t = 1.0f;
			if (t < 0.0f) t = 0.0f;
			//t = 0.0f;
			return currentKey->second.getVQS() *(1- t) + next->second.getVQS() * t;
		}
		return currentKey->second.getVQS();
	}
	else return bone->getVQS();
}

SEVQS SEAnimation::getTransformSLerped() {
	if (keyFrames.size() > 0) {
		auto next = currentKey;
		if (++next != keyFrames.end()) {
			float currentFrame = timer.TotalTime() * SE_ANIMATION_FPS;
			float t = (currentFrame - currentKey->first) / (next->first - currentKey->first);
			if (t > 1.0f) t = 1.0f;
			if (t < 0.0f) t = 0.0f;
			SEVQS vqs1 = currentKey->second.getVQS();
			SEVQS vqs2 = next->second.getVQS();
			XMVECTOR v1 = vqs1.getV();
			XMVECTOR v2 = vqs2.getV();
			SEQuaternion q1 = vqs1.getQ();
			SEQuaternion q2 = vqs2.getQ();
			float s1 = vqs1.getS();
			float s2 = vqs2.getS();
			float dot = q1.dot(q2);
			if (fabsf(dot - 1.0f) < 0.000001f) return vqs1;
			float alpha = acosf(q1.dot(q2));
			return SEVQS(
				v1 * (1 - t) + v2*t,
				(q1 *sinf(alpha*(1.0f - t)) + q2 * sinf(alpha * t)) / sinf(alpha),
				s1 * pow(s2/s1, t)
			);
		}
		return currentKey->second.getVQS();
	}
	else return bone->getVQS();
}

SEVQS SEAnimation::getTransformiSLerped() {
	if (keyFrames.size() > 0) {
		auto next = currentKey;
		if (++next != keyFrames.end()) {
			float currentFrame = timer.TotalTime() * SE_ANIMATION_FPS;

			if (currentFrame - currentKey->first > stepTime) {
				stepTime += ((next->first - currentKey->first) / SE_NUM_INTERPOLATION_STEP);
				currentStep.increment(XMLoadFloat3(&stepV), stepQ, stepS.x);
				return currentStep;
			}
			else return currentStep;
		}
		return currentKey->second.getVQS();
	}
	else return bone->getVQS();
}

tangentType & SEKeyFrame::tan(elementType type) {
	switch (type) {
	case SE_ELEMENT_TX: return tan_tx;
	case SE_ELEMENT_TY: return tan_ty;
	case SE_ELEMENT_TZ: return tan_tz;
	case SE_ELEMENT_RX: return tan_rx;
	case SE_ELEMENT_RY: return tan_ry;
	case SE_ELEMENT_RZ: return tan_rz;
	case SE_ELEMENT_SX: return tan_sx;
	case SE_ELEMENT_SY: return tan_sy;
	case SE_ELEMENT_SZ: return tan_sz;
	}

	return tan_undefine;
}

float *& SEKeyFrame::key(elementType type) {
	switch (type) {
	case SE_ELEMENT_TX:	return key_tx;
	case SE_ELEMENT_TY:	return key_ty;
	case SE_ELEMENT_TZ:	return key_tz;
	case SE_ELEMENT_RX:	return key_rx;
	case SE_ELEMENT_RY:	return key_ry;
	case SE_ELEMENT_RZ:	return key_rz;
	case SE_ELEMENT_SX:	return key_sx;
	case SE_ELEMENT_SY:	return key_sy;
	case SE_ELEMENT_SZ:	return key_sz;
	}
	return key_undefine;
}

float * SEKeyFrame::getKey(elementType type) const {
	switch (type) {
	case SE_ELEMENT_TX:	return key_tx;
	case SE_ELEMENT_TY:	return key_ty;
	case SE_ELEMENT_TZ:	return key_tz;
	case SE_ELEMENT_RX:	return key_rx;
	case SE_ELEMENT_RY:	return key_ry;
	case SE_ELEMENT_RZ:	return key_rz;
	case SE_ELEMENT_SX:	return key_sx;
	case SE_ELEMENT_SY:	return key_sy;
	case SE_ELEMENT_SZ:	return key_sz;
	}
	return nullptr;
}

float SEKeyFrame::getKeyValue(elementType type) const {
	switch (type) {
	case SE_ELEMENT_TX:	return *key_tx;
	case SE_ELEMENT_TY:	return *key_ty;
	case SE_ELEMENT_TZ:	return *key_tz;
	case SE_ELEMENT_RX:	return *key_rx;
	case SE_ELEMENT_RY:	return *key_ry;
	case SE_ELEMENT_RZ:	return *key_rz;
	case SE_ELEMENT_SX:	return *key_sx;
	case SE_ELEMENT_SY:	return *key_sy;
	case SE_ELEMENT_SZ:	return *key_sz;
	}
	return 0.0f;
}

tangentType SEKeyFrame::getTan(elementType type) const {
	switch (type) {
	case SE_ELEMENT_TX: return tan_tx;
	case SE_ELEMENT_TY: return tan_ty;
	case SE_ELEMENT_TZ: return tan_tz;
	case SE_ELEMENT_RX: return tan_rx;
	case SE_ELEMENT_RY: return tan_ry;
	case SE_ELEMENT_RZ: return tan_rz;
	case SE_ELEMENT_SX: return tan_sx;
	case SE_ELEMENT_SY: return tan_sy;
	case SE_ELEMENT_SZ: return tan_sz;
	}

	return SE_TANGENT_UNKNOWN;
}

bool SEKeyFrame::lerpAttri(elementType type, int frame, std::map<int, SEKeyFrame>::iterator nullIter, float *pData) const {
	if (getKey(type)) *pData = getKeyValue(type);	// we have the value, no need to lerp
	else {
		// find the closet previous/post keys that have key value
		auto l = last, n = next;
		while (l != nullIter && l->second.getKey(type) == nullptr) l = l->second.last;
		while (n != nullIter && n->second.getKey(type) == nullptr) n = n->second.next;

		if (n == nullIter) {
			if (l == nullIter) return false;		// no keys to interpolate, use local
			else *pData = l->second.getKeyValue(type);	// only previous key exists, use it
		}
		else {
			if (l == nullIter) *pData = n->second.getKeyValue(type);	// only next key exists, use it
			else {
				// both cloest keys exist, lerp them
				float t = float(frame - l->first) / float(n->first - l->first);
				*pData = l->second.getKeyValue(type) * (1 - t) + n->second.getKeyValue(type) * t;
			}
		}
	}

	return true;
}

SEKeyFrame::SEKeyFrame(elementType type, SEBone *_bone):
	bone(_bone),
	eType(type),
	key_tx(nullptr),
	key_ty(nullptr),
	key_tz(nullptr),
	key_rx(nullptr),
	key_ry(nullptr),
	key_rz(nullptr),
	key_sx(nullptr),
	key_sy(nullptr),
	key_sz(nullptr),
	key_undefine(nullptr),
	tan_tx(SE_TANGENT_UNKNOWN),
	tan_ty(SE_TANGENT_UNKNOWN),
	tan_tz(SE_TANGENT_UNKNOWN),
	tan_rx(SE_TANGENT_UNKNOWN),
	tan_ry(SE_TANGENT_UNKNOWN),
	tan_rz(SE_TANGENT_UNKNOWN),
	tan_sx(SE_TANGENT_UNKNOWN),
	tan_sy(SE_TANGENT_UNKNOWN),
	tan_sz(SE_TANGENT_UNKNOWN),
	tan_undefine(SE_TANGENT_UNKNOWN)
{ }

SEKeyFrame::~SEKeyFrame() {
	if (key_tx) delete key_tx;
	if (key_ty) delete key_ty;
	if (key_tz) delete key_tz;
	if (key_rx) delete key_rx;
	if (key_ry) delete key_ry;
	if (key_rz) delete key_rz;
	if (key_sx) delete key_sx;
	if (key_sy) delete key_sy;
	if (key_sz) delete key_sz;
	if (key_undefine) delete key_undefine;
}

SEKeyFrame & SEKeyFrame::operator=(const SEKeyFrame & rhs) {
	bone = rhs.bone;
	elementType type = rhs.eType;
	float *rhsValue = rhs.getKey(type), *&lhsValue = key(type);
	if (rhsValue) {
		if (lhsValue) *lhsValue = *rhsValue;
		else lhsValue = new float(*rhsValue);
	}
	tan(rhs.eType) = rhs.getTan(rhs.eType);
	return *this;
}

const SEVQS & SEKeyFrame::constructVQS(int frame, std::map<int, SEKeyFrame>::iterator nullIter) {
	vqs = SEVQS();
	float tx, ty, tz, rx, ry, rz, sx, sy, sz;
	if (!lerpAttri(SE_ELEMENT_TX, frame, nullIter, &tx)) tx = bone->getLocalTranslation().x;
	if (!lerpAttri(SE_ELEMENT_TY, frame, nullIter, &ty)) ty = bone->getLocalTranslation().y;
	if (!lerpAttri(SE_ELEMENT_TZ, frame, nullIter, &tz)) tz = bone->getLocalTranslation().z;
	if (!lerpAttri(SE_ELEMENT_RX, frame, nullIter, &rx)) rx = bone->getLocalRotation().x;
	if (!lerpAttri(SE_ELEMENT_RY, frame, nullIter, &ry)) ry = bone->getLocalRotation().y;
	if (!lerpAttri(SE_ELEMENT_RZ, frame, nullIter, &rz)) rz = bone->getLocalRotation().z;
	//if (!lerpAttri(SE_ELEMENT_SX, frame, nullIter, &sx)) sx = bone->getLocalScale().x;
	//if (!lerpAttri(SE_ELEMENT_SY, frame, nullIter, &sy)) sy = bone->getLocalScale().y;
	//if (!lerpAttri(SE_ELEMENT_SZ, frame, nullIter, &sz)) sz = bone->getLocalScale().z;

	vqs.translateBy(XMVectorSet(tx, ty, tz, 0.0f));
	vqs.rotateBy(XMVectorSet(rx, ry, rz, 0.0f));
	return vqs;
}

void SEKeyFrame::setTangent(elementType _eType, tangentType _tType) {
	tan(_eType) = _tType;
}

void SEKeyFrame::setValue(elementType _eType, float value) {
	if (_eType == SE_ELEMENT_RX || _eType == SE_ELEMENT_RY || _eType == SE_ELEMENT_RZ) value = XMConvertToRadians(value);
	if (getKey(_eType)) *getKey(eType) = value;
	else key(_eType) = new float(value);
}
