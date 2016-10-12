#pragma once

#include "d3dUtil.h"
#include <vector>

class SEBone
{
public:
	SEBone() {}
	~SEBone();

	void loadBone(std::string _name, DirectX::XMFLOAT3 trans, DirectX::XMFLOAT3 _orient);
	void addChild(SEBone *child);
	void drawAll();

private:
	std::string name;
	std::vector<SEBone*> children;
	DirectX::XMFLOAT3 localTranslation;
	DirectX::XMFLOAT3 orient;
};