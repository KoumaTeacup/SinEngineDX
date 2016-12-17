#pragma once

#include "d3dUtil.h"
#include "asset.h"


class SESpring : public SEAsset
{
public:
	void Bind();
	void virtual Tick(float dt);
	void virtual Draw();

	SESpring();
	~SESpring();

	void ConstructSpring(SEAsset * a0o, int MassVertexID0, SEAsset * a1o, int MassVertexID1);
	DirectX::FXMVECTOR computeForce(MassVertex *end) const;

private:
	float length;
	float SpringConstant;

	MassVertex *anchor0;
	SEAsset *anchor0Object;
	MassVertex *anchor1;
	SEAsset *anchor1Object;

	ID3D11Buffer *buffer;
};