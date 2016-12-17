#pragma once

#include <DirectXMath.h>

#include <vector>
#include "quaternion.h"

struct arcLengthTableEntry
{
	float u;
	float arcLength;
	arcLengthTableEntry(float f0, float f1) : u(f0), arcLength(f1) {}
};

class SEAsset;
class SESpline;

class SEMovement
{
public:
	SEMovement();
	~SEMovement() {}

	void AssignCurve(SESpline *_pathCurve);
	void EnablePath();
	void DisablePath() { followCurve = false; }
	void AddWorldTranslate(DirectX::FXMVECTOR trans);
	void SetWorldTranslate(DirectX::FXMVECTOR trans) { XMStoreFloat3(&worldTranslate,  trans); }
	void SetScale(DirectX::FXMVECTOR scale);
	void Reset() { accumulatedTime = 0.0f; }

	DirectX::FXMVECTOR getWorldTranslate();
	DirectX::FXMVECTOR getWorldScale();
	SEQuaternion &getRotationRef() { return worldRotation; }

	void computePhysicsRotation(DirectX::FXMVECTOR omega);

	void Tick(float dt, SEAsset *owner);
	void Draw();

	DirectX::XMVECTOR getTickTranslation(float dt);
	void constructTable();

	bool tableReady;
private:
	DirectX::XMVECTOR arcLengthReverse(float s);
	DirectX::XMVECTOR arcLengthReverse(float s, int begin, int end);

	DirectX::XMFLOAT3 worldTranslate;
	DirectX::XMFLOAT3 worldScale;
	SEQuaternion worldRotation;

	bool followCurve;
	SESpline *pathCurve;
	std::vector<arcLengthTableEntry> arcLengthTable;
	float accumulatedTime;
	float curveLength;
};