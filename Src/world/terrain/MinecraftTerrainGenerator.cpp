#include "world/terrain/MinecraftTerrainGenerator.h"

#include <iostream>

#include "world/Chunk.h"

MinecraftNoiseGenerator::MinecraftNoiseGenerator(unsigned int seed):
	seaBed(16),
	continentalnessGenerator(OctaveNoiseGenerator(seed, 8, 0.0005, 0.015)),
	continentalnessInterpolator(LinearInterpolator({
		{-0.9, 0}, {-0.7, 0.01}, {-0.5, 0.2}, {0, 0.3}, {0.2, 0.4}, {0.3, 0.5}, {0.5, 0.6}, {0.7, 0.9}, {0.8, 0.95}, {1, 1}
		})),
	erosionGenerator(OctaveNoiseGenerator(seed * 32, 16, 0.005, 0.01)),
	erosionInterpolator(LinearInterpolator({
		{-1, -0.9}, {-0.9, 0.8}, {-0.7, 0.5}, {-0.65, 0.55}, {-0.1, 0.2},
		{0.45, 0.3}, {0.55, 0.4}, {0.65, 0.1}, {0.7, 0.2 }
		}))
{}

unsigned char* MinecraftNoiseGenerator::generateTerrain(unsigned int seed, const Vector& position, const std::unordered_map<Vector, unsigned char>& pendingBlocks) const
{
	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	int seaLevel = 40;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = int(position.x + x);

		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = int(position.z + z);

			int height = getHeight(globalX, globalZ);

			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int globalY = int(position.y + y);

				bool isBlockAir = globalY > height;

				if (isBlockAir && (globalY <= seaLevel))
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = globalY == seaLevel ? BLOCK_WATER_SURFACE : BLOCK_WATER;
				}
				else
				{
					if (globalY == height)
					{
						blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = (globalY < seaLevel) ? BLOCK_DIRT : BLOCK_GRASS;
					}
					else if ((globalY >= height - 3) && (globalY <= height - 1))
					{
						blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_DIRT;
					}
					else
					{
						blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = isBlockAir ? BLOCK_AIR : BLOCK_STONE;
					}
				}
			}
		}
	}

	for (auto& [position, blockID] : pendingBlocks)
	{
		std::cout << position << " adding " << int(blockID) << std::endl;
		blocks[int(position.y * CHUNK_WIDTH * CHUNK_WIDTH + position.x * CHUNK_WIDTH + position.z)] = blockID;
	}

	return blocks;
}

int MinecraftNoiseGenerator::getHeight(int x, int z) const
{
	int maxHeight = CHUNK_HEIGHT - seaBed;

	double continentalness = continentalnessGenerator.get(x, z);
	double continentalnessFactor = continentalnessInterpolator.interpolate(continentalness);

	double erosion = erosionGenerator.get(x, z);
	double erosionFactor = erosionInterpolator.interpolate(erosion);
	
	return seaBed + std::min(int(continentalnessFactor * maxHeight), CHUNK_HEIGHT);
}