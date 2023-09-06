#include <renderer/font/Glyph.h>

Glyph::Glyph(int width, int height, float minU, float minV, float maxU, float maxV) :
	width(width), height(height), minU(minU), minV(minV), maxU(maxU), maxV(maxV)
{
}

int Glyph::getWidth() const
{
	return width;
}

int Glyph::getHeight() const
{
	return height;
}

float Glyph::getMinU() const
{
	return minU;
}

float Glyph::getMinV() const
{
	return minV;
}

float Glyph::getMaxU() const
{
	return maxU;
}

float Glyph::getMaxV() const
{
	return maxV;
}