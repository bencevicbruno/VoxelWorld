#pragma once

class Color
{
public:
	Color(const Color& other) = default;
	Color(Color&& other) noexcept = default;
	Color& operator=(const Color& other) = default;
	Color& operator=(Color&& other) noexcept = default;

	Color operator*(const Color& other) const;

	float r, g, b, a;

	static Color CreateFromRGB(int rgb);
	static Color CreateFromRGB(int r, int g, int b);
	static Color CreateFromRGBA(int rgb, int a);
	static Color CreateFromRGBA(int r, int g, int b, int a);

	static Color CreateRandom();
	static Color CreateRandomGray(float minBrightness, float maxBrightness);

private:
	Color(float r, float g, float b, float a);
};
