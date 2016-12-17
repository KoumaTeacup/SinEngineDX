#include "asset.h"

using namespace DirectX;

DirectX::FXMVECTOR SEAsset::computeRigidLocation() {
	XMVECTOR com = rigidbody.getCenterOfMass();
	XMVECTOR wtrans = movement.getWorldTranslate();
	return com + wtrans;
}

void SEAsset::Draw() {
	movement.Draw();
}

void SEAsset::Tick(float dt) {
	movement.Tick(dt, this);
}

void SEAsset::PhysicsTick(float dt) {
	if(!rigidbody.isEnabled()) return;
	movement.AddWorldTranslate(rigidbody.computeLinearDisplacement(dt) * 1.0f);
	movement.computePhysicsRotation(rigidbody.computeAngularDisplacement(dt, movement.getRotationRef()));
}
