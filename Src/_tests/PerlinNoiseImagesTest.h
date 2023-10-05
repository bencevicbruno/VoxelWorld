#pragma once

#include "_tests/TestUtils.h"

#include <ctime>

#include "utils/image/ImageManager.h"
#include "math/LinearInterpolator.h"

namespace {
	void performPerlinNoiseTest1()
	{
		ImageManager& imageManager = ImageManager::GetInstance();

		const int imageSize = 1024;

		for (int i = 0; i < 10; i++) {
			Image image = std::move(Image::CreateFromPerlinNoise(imageSize, i, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 10));
			//Image image = std::move(Image::CreateFromPerlinNoise(imageSize, i, [](double value) -> unsigned char { return value > 0.45 ? 255 : 0; }));

			std::string imageName = std::string("test/perlin_noise_") + std::to_string(i) + ".png";
			std::cout << imageName << std::endl;
			imageManager.saveImage(image, imageName);
		}
	}

	void performPerlinNoiseTest2()
	{
		ImageManager& imageManager = ImageManager::GetInstance();

		// examples
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20), "test/perlin_noise_example1.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 123, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20), "test/perlin_noise_example2.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 456, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20), "test/perlin_noise_example3.png");
	
		// conditions
		//imageManager.saveImage(Image::CreateFromPerlinNoise(2048, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
		//	{
		//		return value * 255;
		//	}), "test/perlin_noise_example_condition1.png");
		//imageManager.saveImage(Image::CreateFromPerlinNoise(2048, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
		//	{
		//		return value > 0.5 ? 0 : 255;
		//	}), "test/perlin_noise_example_condition2.png");
		//imageManager.saveImage(Image::CreateFromPerlinNoise(2048, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
		//	{
		//		return value >= 0.4 && value <= 0.6 ? 255 : 0;
		//	}), "test/perlin_noise_example_condition3.png");
		//imageManager.saveImage(Image::CreateFromPerlinNoise(2048, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
		//	{
		//		return value < 0.33 ? 0 : value < 0.66 ? 127 : 255;
		//	}), "test/perlin_noise_example_condition4.png");

		// interpolated
		
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 0, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
			{
				LinearInterpolator interpolator({ { 0.0, 0.0 }, { 0.25, 0.2 }, { 0.65, 0.33 }, { 0.85, 0.4 }, { 1.0, 1.0 } });
				return interpolator.interpolate(value) * 255;
			}), "test/perlin_noise_example_interpolated_1.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 123, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
			{
				LinearInterpolator interpolator({ { 0.0, 0.0 }, { 0.25, 0.2 }, { 0.65, 0.33 }, { 0.85, 0.4 }, { 1.0, 1.0 } });
				return interpolator.interpolate(value) * 255;
			}), "test/perlin_noise_example_interpolated_2.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(1024, 456, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20, [](double value) -> unsigned char
			{
				LinearInterpolator interpolator({ { 0.0, 0.0 }, { 0.25, 0.2 }, { 0.65, 0.33 }, { 0.85, 0.4 }, { 1.0, 1.0 } });
				return interpolator.interpolate(value) * 255;
			}), "test/perlin_noise_example_interpolated_3.png");
	}

	void performPerlinNoiseTest3()
	{
		ImageManager& imageManager = ImageManager::GetInstance();

		int size = 800;
		int seed = 1024;

		imageManager.saveImage(Image::CreateFromPerlinNoise(size, seed, FastNoiseLite::NoiseType::NoiseType_Perlin, 20), "test/fast_noise_perlin.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(size, seed, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 20), "test/fast_noise_simplex.png");
		imageManager.saveImage(Image::CreateFromPerlinNoise(size, seed, FastNoiseLite::NoiseType::NoiseType_Cellular, 20), "test/fast_noise_cellular.png");
	}

	void performPerlinNoiseTest4()
	{
		ImageManager& imageManager = ImageManager::GetInstance();
		Image image(8 * 16, 8 * 16);

		PerlinNoiseGenerator continentalnessGenerator(0, 0.8);
		LinearInterpolator continentalnessInterpolator({
			//{0.0, 0.0}, {0.5, 0.5}, {1.0, 0.0}
			//{0.0, 1.0}, {0.15, 0.1}, {0.25, 0.15}, {1.0, 0.4}
			{0.0, 0.0}, {0.25, 0.2}, {0.65, 0.33}, {0.85, 0.4}, {1.0, 1.0}
			});
		PerlinNoiseGenerator erosionGenerator(0 << 4, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 0.5, 1, 0.5);
		LinearInterpolator erosionInterpolator({
				{0.0, -0.25}, {0.5, 0}, {1.0, 0.25}
			});

		for (int x = 0; x < 8 * 16; x++)
		{
			for (int z = 0; z < 8 * 16; z++)
			{
				double continentalness = continentalnessGenerator.get(x, z);
				double continentalnessFactor = continentalnessInterpolator.interpolate(continentalness);

				double erosion = erosionGenerator.get(x, z);
				double erosionFactor = erosionInterpolator.interpolate(erosion);

				double value = Math::Clamp(continentalnessFactor + erosionFactor, 0, 1.0);
				image.data[4 * (8 * 16 * z + x) + 0] = value * 255;
				image.data[4 * (8 * 16 * z + x) + 1] = value * 255;
				image.data[4 * (8 * 16 * z + x) + 2] = value * 255;
				image.data[4 * (8 * 16 * z + x) + 3] = 255;
			}
		}

		imageManager.saveImage(image, "test/seed0.png");

	}
}

void performPerlinNoiseTests()
{
	//performPerlinNoiseTest1();
	//performPerlinNoiseTest2();
	//performPerlinNoiseTest3();
	performPerlinNoiseTest4();
}