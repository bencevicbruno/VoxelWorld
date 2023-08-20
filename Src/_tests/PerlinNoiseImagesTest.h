#pragma once

#include "_tests/TestUtils.h"

#include <ctime>

#include "utils/image/ImageManager.h"

namespace {
	void performPerlinNoiseTest1()
	{
		ImageManager& imageManager = ImageManager::GetInstance();

		const int imageSize = 64;

		for (int i = 0; i < 10; i++) {
			Image image = std::move(Image::CreateFromPerlinNoise(imageSize, i));
			//Image image = std::move(Image::CreateFromPerlinNoise(imageSize, i, [](double value) -> unsigned char { return value > 0.45 ? 255 : 0; }));

			std::string imageName = std::string("test/perlin_noise_") + std::to_string(i) + ".png";
			std::cout << imageName << std::endl;
			imageManager.saveImage(image, imageName);
		}
	}

	void performPerlinNoiseTest2()
	{
		ImageManager& imageManager = ImageManager::GetInstance();
		imageManager.saveImage(Image::CreateFromPerlinNoise(2048, 0, [](double value) -> unsigned char { return value > 0.5 ? 255 : 0; }), "test/perlin_noise_big.png");
	}
}

void performPerlinNoiseTests()
{
	performPerlinNoiseTest2();
}