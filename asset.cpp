#include "asset.h"

void SEAsset::Draw() {
	movement.Draw();
}

void SEAsset::Tick(float dt) {
	movement.Tick(dt, this);
}
