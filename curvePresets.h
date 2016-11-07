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
	CurveControlPoint(-5.500, 0.0, -22.000),
	CurveControlPoint(-11.00, 0.0, -15.399),
	CurveControlPoint(-16.50, 0.0, -4.400),
	CurveControlPoint(-25.29, 0.0, -13.200),
	CurveControlPoint(-13.20, 0.0, 6.600),
	CurveControlPoint(5.500 ,0.0 ,-12.649),
	CurveControlPoint(22.00 ,0.0 ,-5.500),
	CurveControlPoint(1.650 ,0.0 ,4.400),
	CurveControlPoint(-2.200, 0.0, -13.200),
	CurveControlPoint(11.00 ,0.0 ,14.850),
	CurveControlPoint(24.75 ,0.0 ,8.250),
	CurveControlPoint(27.50 ,0.0 ,-1.100)
};

const UINT simpleCurveNum = 4;
const CurveControlPoint simpleCurve[simpleCurveNum] = {
	CurveControlPoint(0, 10, 0),
	CurveControlPoint(10, 20, 0),
	CurveControlPoint(10, 30, 0),
	CurveControlPoint(0, 40, 0)
};


}