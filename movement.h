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
	void EnablePath() { followCurve = true; }
	void DisablePath() { followCurve = false; }

	void Tick(float dt, SEAsset *owner);
	void Draw();

private:
	void constructTable();
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