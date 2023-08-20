#include "world/terrain/MinecraftTerrainGenerator.h"

#include <iostream>

#include "world/Chunk.h"

MinecraftNoiseGenerator::MinecraftNoiseGenerator(unsigned int seed):
	continentalnessGenerator(OctaveNoiseGenerator(seed, 8, 0.0005, 0.015)),
	continentalnessInterpolator(LinearInterpolator({
		{-0.9, 1}, {-0.7, 0.01}, {-0.5, 0.2}, {0, 0.3}, {0.2, 0.4}, {0.3, 0.5}, {0.5, 0.6}, {0.7, 0.9}, {0.8, 0.95}, {1, 1} })),
	erosionGenerator(OctaveNoiseGenerator(seed, 16, 0.00005, 0.01)),
	erosionInterpolator(LinearInterpolator({
		{-1, 1}, {-0.9, 0.8}, {-0.7, 0.5}, {-0.65, 0.55}, {-0.1, 0.2}, {0.4, 0.15}, {0.45, 0.4}, {0.55, 0.4}, {0.65, 0.15}, {0.7, 0.01} })),
	peaksAndValleysGenerator(OctaveNoiseGenerator(seed, 12, 0.2, 0.1)),
	peaksAndValleysInterpolator(LinearInterpolator({
		{-0.9, 0.0}, {-0.8, 0.1}, {-0.7, 0.25}, {-0.3, 0.45}, {0, 0.5}, {0.75, 0.75}, {0.8, 0.8}, {0.9, 1.0} }))
{}

unsigned char* MinecraftNoiseGenerator::generateTerrain(unsigned int seed, const Vector& position) const
{
	unsigned char* blocks = new unsigned char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

	int seaLevel = 16;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		int globalX = int(position.x + x);

		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			int globalZ = int(position.z + z);

			int height = getHeight(globalX, globalZ);

			//std::cout << "Height at " << globalX << "/" << globalZ << ": " << height << std::endl;

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int globalY = int(position.y + y);

				bool isBlockAir = globalY > height;

				if (isBlockAir && (globalY <= seaLevel))
				{
					blocks[x * 16 * 16 + z * 16 + y] = globalY == seaLevel ? BLOCK_WATER_SURFACE : BLOCK_WATER;
				}
				else
				{
					if (globalY == height)
					{
						blocks[x * 16 * 16 + z * 16 + y] = (globalY < seaLevel) ? BLOCK_DIRT : BLOCK_GRASS;
					}
					else if ((globalY >= height - 3) && (globalY <= height - 1))
					{
						blocks[x * 16 * 16 + z * 16 + y] = BLOCK_DIRT;
					}
					else
					{
						blocks[x * 16 * 16 + z * 16 + y] = isBlockAir ? BLOCK_AIR : BLOCK_STONE;
					}
				}
			}
		}
	}

	return blocks;
}

int MinecraftNoiseGenerator::getHeight(int x, int z) const
{
	constexpr int maxHeight = 64;

	double continentalness = continentalnessGenerator.get(x, z);
	double continentalnessFactor = continentalnessInterpolator.interpolate(continentalness);

	double erosion = erosionGenerator.get(x, z);
	double erosionFactor = erosionInterpolator.interpolate(erosion);

	double peaksAndValleys = peaksAndValleysGenerator.get(x, z);
	double peaksAndValleysFactor = peaksAndValleysInterpolator.interpolate(peaksAndValleys);
	
	return int(continentalnessFactor * maxHeight);
}