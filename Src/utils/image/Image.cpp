#include "utils/image/Image.h"

#include <iostream>
#include <sstream>

#include "math/Math.h"
#include "math/PerlinNoiseGenerator.h"
#include "utils/Utils.h"

#include "utils/image/ImageManager.h"

Image::Image() :
	width(0), height(0), data(nullptr), isRGBA(false)
{}

Image::Image(int size) :
	width(size), height(size), data(nullptr), isRGBA(true)
{
	data = new unsigned char[size * size * 4];
}

Image::Image(int width, int height) :
	width(width), height(height), data(0), isRGBA(true)
{
	data = new unsigned char[width * height * 4];
}

Image::Image(Image&& other) noexcept
{
	*this = std::move(other);
}

Image& Image::operator=(Image&& other) noexcept
{
	if (this == &other) return *this;

	width = std::exchange(other.width, 0);
	height = std::exchange(other.height, 0);
	data = std::exchange(other.data, nullptr);
	isRGBA = std::exchange(other.isRGBA, false);

	return *this;
}

Image::~Image()
{
	delete data;
}

Image Image::CreateFromColor(const Color& color)
{
	Image image(1);

	image.data[0] = (unsigned char)(color.r * 225.0f);
	image.data[1] = (unsigned char)(color.g * 225.0f);
	image.data[2] = (unsigned char)(color.b * 225.0f);
	image.data[3] = (unsigned char)(color.a * 225.0f);

	return image;
}

Image Image::CreateFromPerlinNoise(int size, int seed, FastNoiseLite::NoiseType noiseType, double constant)
{
	return Image::CreateFromPerlinNoise(size, seed, noiseType, constant, [](double value) -> unsigned char {
		return (unsigned char)(value * 255);
		});
}

Image Image::CreateFromPerlinNoise(int size, int seed, FastNoiseLite::NoiseType noiseType, double constant, unsigned char (*lambda)(double))
{
	Image image(size);
	PerlinNoiseGenerator noiseGenerator(seed, noiseType, constant);

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			unsigned char value = lambda(noiseGenerator.get(constant * (float)j / (float)size, constant * (float)i / (float)size));
			image.data[4 * (size * i + j) + 0] = value;
			image.data[4 * (size * i + j) + 1] = value;
			image.data[4 * (size * i + j) + 2] = value;
			image.data[4 * (size * i + j) + 3] = 255;
		}
	}

	return image;
}