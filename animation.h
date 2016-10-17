#pragma once

#include <map>

#include "fbxsdk.h"
#include "GameTimer.h"
#include "VQS.h"
#include "d3dUtil.h"

class SEBone;

#define SE_ANIMATION_FPS 24.0f
#define SE_NUM_INTERPOLATION_STEP 3.0f

enum tangentType
{
	SE_TANGENT_LINEAR,
	SE_TANGENT_CUBIC,
	SE_TANGENT_UNKNOWN
};

enum elementType
{
	SE_ELEMENT_TX,
	SE_ELEMENT_TY,
	SE_ELEMENT_TZ,
	SE_ELEMENT_RX,
	SE_ELEMENT_RY,
	SE_ELEMENT_RZ,
	SE_ELEMENT_SX,
	SE_ELEMENT_SY,
	SE_ELEMENT_SZ,
	SE_ELEMENT_UNDEFINED
};

class SEKeyFrame
{
public:
	SEKeyFrame(elementType type = SE_ELEMENT_UNDEFINED, SEBone *_bone = nullptr);
	~SEKeyFrame();

	SEKeyFrame(const SEKeyFrame& rhs) = delete;

	SEKeyFrame &operator=(const SEKeyFrame &rhs);

	const SEVQS &constructVQS(int frame, std::map<int, SEKeyFrame>::iterator nullIter);
	SEVQS getVQS() const { return vqs; }
	void setTangent(elementType _eType, tangentType _tType);
	void setValue(elementType _eType, float value);

	std::map<int, SEKeyFrame>::iterator last, next;
private:
	elementType eType;
	SEBone *bone;
		
	float
		*key_tx,
		*key_ty,
		*key_tz,
		*key_rx,
		*key_ry,
		*key_rz,
		*key_sx,
		*key_sy,
		*key_sz,
		*key_undefine;
	tangentType tan_tx, tan_ty, tan_tz, tan_rx, tan_ry, tan_rz, tan_sx, tan_sy, tan_sz, tan_undefine;


	float *&key(elementType type);
	float *getKey(elementType type) const;
	float getKeyValue(elementType type) const;
	tangentType &tan(elementType type);
	tangentType getTan(elementType type) const;
	bool lerpAttri(elementType type, int frame, std::map<int, SEKeyFrame>::iterator nullIter, float *pData) const;

	SEVQS vqs;
};

class SEAnimation
{
public:
	SEAnimation(SEBone *_bone) :bone(_bone) {}
	void Init();
	void Reset();
	void Pause();
	void Restart();
	void Tick();

	void insert(FbxAnimCurve *curve, elementType eType, FbxTime::EMode timeMode);

	SEVQS getTransformLerped();
	SEVQS getTransformSLerped();
	SEVQS getTransformiSLerped();

private:
	float stepTime;
	DirectX::XMFLOAT3 stepV;
	SEQuaternion stepQ;
	DirectX::XMFLOAT3 stepS;
	SEVQS currentStep;
	SEBone *bone;
	std::map<int, SEKeyFrame>::iterator currentKey;
	GameTimer timer;
	std::map<int, SEKeyFrame> keyFrames;
};