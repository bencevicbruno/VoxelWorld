#pragma once

#include <string>
#include "utils/Color.h"

class Image
{
public:
	int width, height;
	unsigned char* data;
	bool isRGBA;

	Image();
	Image(int size);
	Image(int width, int height);
	Image(Image&& other) noexcept;

	Image& operator=(Image&& other) noexcept;

	~Image();

	static Image CreateFromColor(const Color& color);
	static Image CreateFromPerlinNoise(int size, int seed);
	static Image CreateFromPerlinNoise(int size, int seed, unsigned char (*lambda)(double));
private:
	Image(const Image& other) = delete;
	void operator=(const Image& other) = delete;
};

