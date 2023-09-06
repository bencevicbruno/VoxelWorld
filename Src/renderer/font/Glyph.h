#pragma once

struct Glyph
{
public:
	Glyph(int width, int height, float minU, float minV, float maxU, float maxV);

	int getWidth() const;
	int getHeight() const;
	float getMinU() const;
	float getMinV() const;
	float getMaxU() const;
	float getMaxV() const;

private:
	int width, height;
	float minU, minV;
	float maxU, maxV;

	Glyph(const Glyph& other) = delete;
	Glyph(Glyph&& other) = delete;
	void operator=(const Glyph& other) = delete;
	void operator=(Glyph&& other) = delete;
};
