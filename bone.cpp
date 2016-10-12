#include "bone.h"

SEBone::~SEBone() {
	for (auto i : children) {
		delete i;
	}
}

void SEBone::loadBone(std::string _name, DirectX::XMFLOAT3 trans, DirectX::XMFLOAT3 _orient) {
	name = _name;
	localTranslation = trans;
	orient = _orient;
}

void SEBone::addChild(SEBone * child) {
	children.push_back(child);
}

void SEBone::drawAll() {

}
