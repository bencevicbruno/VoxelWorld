#include "math/Math.h"

float Math::CreateRandom()
{
	return (float)rand() / RAND_MAX;
}

float Math::CreateRandom(float min, float max)
{
	return Math::CreateRandom() * (max - min) + min;
}

float Math::ToRadians(float degrees)
{
	return degrees * (float) M_PI / 180.0f;
}

float Math::ToDegrees(float radians)
{
	return radians / (float) M_PI * 180.0f;
}

float Math::Clamp(float value, float min, float max)
{
	if (value < min) return min;
	else if (value > max) return max;
	else return value;
}

float Math::Sqrt(float value)
{
	return std::sqrt(value);
}

float Math::Sin(float degrees)
{
	return std::sin(ToRadians(degrees));
}

float Math::Cos(float degrees)
{
	return std::cos(ToRadians(degrees));
}

float Math::Tan(float degrees)
{
	return std::tan(ToRadians(degrees));
}

float Math::Arcsin(float sin)
{
	float angle = std::asin(sin);

	return ToDegrees(angle);
}

float Math::Arccos(float cos)
{
	float angle = std::acos(cos);

	return ToDegrees(angle);
}

float Math::Arctan2(float num, float den)
{
	float angle = std::atan2(num, den);

	return ToDegrees(angle);
}