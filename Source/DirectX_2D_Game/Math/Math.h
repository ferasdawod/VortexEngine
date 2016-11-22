#pragma once

#include <DirectXTK/SimpleMath.h>
#include "Random.h"

#define RAD_TO_DEG(x) ((x) * 180.0f / DirectX::XM_PI)
#define DEG_To_RAD(x) ((x) * DirectX::XM_PI / 180.0f)

class Math
{
public:
	static Random	g_Random;

	static float	WrapAngleRadians(float angle);
	static float	WrapAngleDegrees(float angle);
	
	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float	AngleFromXY(float x, float y);
};