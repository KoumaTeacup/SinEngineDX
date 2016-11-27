#pragma once

#include "spline.h"
#include <DirectXMath.h>

namespace SE_Internal_Curve 
{

const UINT testCurveNum = 12;
const CurveControlPoint testCurve[testCurveNum] = {
/*	CurveControlPoint(-300, 1, 0),
	CurveControlPoint(-370, 1, 142),
	CurveControlPoint(-228, 1, 293),
	CurveControlPoint(-78, 1, 251),
	CurveControlPoint(124, 1, 191),
	CurveControlPoint(388, 1, 30),
	CurveControlPoint(309, 1, -114),
	CurveControlPoint(349, 1, -185),
	CurveControlPoint(115, 1, -272),
	CurveControlPoint(-164, 1, -216),
	CurveControlPoint(-400, 1, -225),
	CurveControlPoint(-335, 1, -515),
	CurveControlPoint(-200, 1, -55),
	CurveControlPoint(-300, 1, -0)*/
	CurveControlPoint(-5.500f, 0.0f, -22.000f),
	CurveControlPoint(-11.00f, 0.0f, -15.399f),
	CurveControlPoint(-16.50f, 0.0f, -4.400f),
	CurveControlPoint(-25.29f, 0.0f, -13.200f),
	CurveControlPoint(-13.20f, 0.0f, 6.600f),
	CurveControlPoint(5.500f ,0.0f ,-12.649f),
	CurveControlPoint(22.00f ,0.0f ,-5.500f),
	CurveControlPoint(1.650f ,0.0f ,4.400f),
	CurveControlPoint(-2.200f, 0.0f, -13.200f),
	CurveControlPoint(11.00f ,0.0f ,14.850f),
	CurveControlPoint(24.75f ,0.0f ,8.250f),
	CurveControlPoint(27.50f ,0.0f ,-1.100f)
};

const UINT simpleCurveNum = 4;
const CurveControlPoint simpleCurve[simpleCurveNum] = {
	CurveControlPoint(0, 10, 0),
	CurveControlPoint(10, 20, 0),
	CurveControlPoint(10, 30, 0),
	CurveControlPoint(0, 40, 0)
};


}