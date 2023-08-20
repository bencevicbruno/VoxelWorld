#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

namespace Math
{
	const float EPSILON = 0.0001f;

	float CreateRandom();
	float CreateRandom(float min, float max);
	float ToRadians(float degrees);
	float ToDegrees(float radians);
	float Clamp(float value, float min, float max);
	float Sqrt(float value);
	float Sin(float degrees);
	float Cos(float degrees);
	float Tan(float degrees);
	float Arcsin(float sin);
	float Arccos(float cos);
	float Arctan2(float num, float den);
}