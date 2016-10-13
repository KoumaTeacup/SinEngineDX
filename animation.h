#pragma once

#include "GameTimer.h"
#include <vector>

enum tangentType
{
	SE_TANGENT_LINEAR,
	SE_TANGENT_FLAT,
};

struct SEFrame
{
	float time;
	float key_tx, key_ty, key_tz, key_rx, key_ry, key_rz, key_sx, key_sy, key_sz;
	tangentType tangent;
};

class SEAnimation
{
public:
	SEAnimation();
	~SEAnimation();
private:
	GameTimer timer;

	std::vector<SEFrame> keyFrames;
};