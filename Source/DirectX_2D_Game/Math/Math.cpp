#include "pch.h"

#include "Math.h"

Random Math::g_Random;

float Math::WrapAngleDegrees(float angleDeg)
{
	float angleRad = DEG_To_RAD(angleDeg);
	angleRad = WrapAngleRadians(angleRad);
	return RAD_TO_DEG(angleRad);
}

float Math::WrapAngleRadians(float angleRad)
{
	if (angleRad > DirectX::XM_2PI)
		angleRad = fmod(angleRad, DirectX::XM_2PI);
	else if (angleRad < 0.0f)
		angleRad = DirectX::XM_2PI - fmod(fabs(angleRad), DirectX::XM_2PI);

	return angleRad;
}

float Math::AngleFromXY(float x, float y)
{
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f * DirectX::XM_PI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + DirectX::XM_PI; // in [0, 2*pi).

	return theta;
}