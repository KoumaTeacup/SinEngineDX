#pragma once

#include "asset.h"
#include <vector>
#include "d3dUtil.h"

struct CurveControlPoint {
	DirectX::XMFLOAT3 pos;
	CurveControlPoint(float x, float y, float z) :pos(x, y, z) {}
	CurveControlPoint(DirectX::FXMVECTOR _pos) { XMStoreFloat3(&pos, _pos); }
};

#define curveDataInputCount 1
struct CurveData
{
	DirectX::XMFLOAT3 position;
	CurveData(float x, float y, float z) :position(x, y, z) {}
	CurveData(DirectX::XMFLOAT3 _pos) :position(_pos) {}
};

class SESpline : public SEAsset 
{
public:
	SESpline();
	~SESpline();

	void loadCP(const int num, const CurveControlPoint *_cpList);
	void loadCP(const int num, DirectX::FXMVECTOR *_cpList);
	//void Init();
	void virtual Tick(float dt);
	void Bind();
	void virtual Draw();

	bool isEmpty() { return CPList.size() == 0; }

	DirectX::XMVECTOR computeCurve(float t);

private:
	void initializeLineSeg();

	UINT smoothness;
	std::vector<CurveControlPoint> CPList;
	DirectX::XMFLOAT4 splineFactorX;
	DirectX::XMFLOAT4 splineFactorY;
	DirectX::XMFLOAT4 splineFactorZ;
	std::vector<float> coefX;
	std::vector<float> coefY;
	std::vector<float> coefZ;

	ID3D11Buffer *buffer;
};