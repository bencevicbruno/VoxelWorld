#include "world/terrain/MinecraftTerrainGenerator.h"

#include <iostream>

#include "utils/debug/Debug.h"
#include "world/chunk/Chunk.h"

MinecraftNoiseGenerator::MinecraftNoiseGenerator(unsigned int seed):
	seaBed(16),
	continentalnessGenerator(PerlinNoiseGenerator(seed, 0.8)),
	continentalnessInterpolator(LinearInterpolator({
		{0.0, 0.0}, {0.25, 0.2}, {0.65, 0.33}, {0.85, 0.4}, {1.0, 1.0}
		})),
	erosionGenerator(PerlinNoiseGenerator(seed, 0.005)),
	erosionInterpolator(LinearInterpolator({
		{-1, -0.9}, {-0.9, 0.8}, {-0.7, 0.5}, {-0.65, 0.55}, {-0.1, 0.2},
		{0.45, 0.3}, {0.55, 0.4}, {0.65, 0.1}, {0.7, 0.2 }
		}))
{}

unsigned char* MinecraftNoiseGenerator::generateTerrain(unsigned int seed, const Vector& position, const std::unordered_map<Vector, unsigned char>& pendingBlocks) const
{
	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	int seaLevel = 32;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = int(16 * position.x + x);

		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = int(16 * position.z + z);

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
						if (globalY < seaLevel - 3)
						{
							blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = (continentalnessGenerator.get(globalX * 10, globalZ * 10) < 0.5) ? BLOCK_GRAVEL : BLOCK_SAND;
						}
						else
						{
							blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = (globalY < seaLevel) ? BLOCK_DIRT : BLOCK_GRASS;
						}
					}
					else if ((globalY >= height - 3) && (globalY <= height - 1))
					{
						blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_DIRT;
					}
					else if ((globalY == height + 1) && (globalY > seaLevel))
					{
						if (std::abs(globalX * globalZ + globalY) % 13 < 2)
						{
							blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_TALL_GRASS;
						}

						if (std::abs(globalY * globalX + globalZ) % 13 < 2)
						{
							blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_FLOWER_YELLOW;
						}

						if (std::abs(globalZ * globalY + globalX) % 13 < 2)
						{
							blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_FLOWER_RED;
						}
					}
					else
					{
						blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = isBlockAir ? BLOCK_AIR : BLOCK_STONE;
					}
				}
			}
		}
	}

	//Debug::printBlocks(pendingBlocks);

	return blocks;
}

int MinecraftNoiseGenerator::getHeight(int x, int z) const
{
	int maxHeight = CHUNK_HEIGHT - seaBed;

	double continentalness = continentalnessGenerator.get(x, z);
	//std::cout << int(continentalnessGenerator.get(x, z) * CHUNK_HEIGHT) << std::endl;
	double continentalnessFactor = continentalnessInterpolator.interpolate(continentalness);

	double erosion = erosionGenerator.get(x, z);
	double erosionFactor = erosionInterpolator.interpolate(erosion);
	
	return seaBed + std::min(int(continentalnessFactor * maxHeight), CHUNK_HEIGHT);
}