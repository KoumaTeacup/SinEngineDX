#pragma once

#include "d3dUtil.h"
#include "VAO.h"
#include "VQS.h"
#include <vector>

class SEAnimation;

class SEBone
{
public:
	SEBone() {}
	~SEBone();

	void loadBone(std::string _name, DirectX::XMFLOAT3 trans, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scl, const SEQuaternion &_orient);
	void addChild(SEBone *child);
	void drawAll(SEBone *parent);
	void addAnimation(SEAnimation *anim) { animations.push_back(anim); }

	static void InitBoneMesh();
	void Reset();
	void Tick();

	const XMFLOAT3 &getLocalTranslation() { return localTranslation; }
	const XMFLOAT3 &getLocalRotation() { return localRotation; }
	const XMFLOAT3 &getLocalScale() { return localScale; }

	const SEVQS &getVQS() { return transLocal; }

private:
	std::string name;
	std::vector<SEBone*> children;
	std::vector<SEAnimation*> animations;
	DirectX::XMFLOAT3 localTranslation;
	DirectX::XMFLOAT3 localRotation;
	DirectX::XMFLOAT3 localScale;
	SEQuaternion orient;
	SEVQS transLocal;
	SEVQS transAnim;
	SEVQS transFinal;
	SEVQS transOrient;

	static SEVAO boneMesh;
	static SEVAO jointMesh;
};