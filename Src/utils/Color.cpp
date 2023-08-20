#include "utils\Color.h"

#include "math/Math.h"

Color Color::operator*(const Color& other) const
{
	return Color(r * other.r, g * other.g, b * other.b, a * other.a);
}

Color Color::CreateFromRGB(int rgb)
{
	return Color::CreateFromRGBA(rgb, rgb, rgb, 255);
}

Color Color::CreateFromRGB(int r, int g, int b)
{
	return Color::CreateFromRGBA(r, g, b, 255);
}

Color Color::CreateFromRGBA(int rgb, int a)
{
	return Color::CreateFromRGBA(rgb, rgb, rgb, a);
}

Color Color::CreateFromRGBA(int r, int g, int b, int a)
{
	return Color(
		(float)r / 255.0f,
		(float)g / 255.0f,
		(float)b / 255.0f,
		(float)a / 255.0f
	);
}

Color Color::CreateRandom()
{
	return Color(
		Math::CreateRandom(),
		Math::CreateRandom(),
		Math::CreateRandom(),
		Math::CreateRandom()
	);
}

Color Color::CreateRandomGray(float minBrightness, float maxBrightness)
{
	float brightness = Math::CreateRandom(minBrightness, maxBrightness);

	return Color(brightness, brightness, brightness, 1.0f);
}

Color::Color(float r, float g, float b, float a) :
	r(r), g(g), b(b), a(a)
{}

