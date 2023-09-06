#include "world/decorator/SurfaceBlockDecorator.h"

#include "world/chunk/Chunk.h"

SurfaceBlockDecorator::SurfaceBlockDecorator(int seed, int lakeLevel) :
	seed(seed),
	lakeLevel(lakeLevel),
	vegetationGenerator(PerlinNoiseGenerator(seed << 1, 1)),
	coarsenessGenerator(PerlinNoiseGenerator(seed << 2, 1))
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> SurfaceBlockDecorator::decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int height = heightMap[CHUNK_WIDTH * x + z];

			if (height <= lakeLevel + 2)
			{
				for (int y = height - 2; y <= height; y++)
				{
					if (blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] == BLOCK_AIR) continue;
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_SAND;
				}
			}
			else if (height > 100)
			{
				for (int y = height - 4; y <= height; y++)
				{
					if (blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] == BLOCK_AIR) continue;

					double coarsenessLevel = coarsenessGenerator.get(5 * (chunkPosition.x * 16 + x), 5 * (chunkPosition.z * 16 + z));
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = y == height ? (coarsenessLevel > 0.5 ? BLOCK_GRAVEL : BLOCK_SNOW) : BLOCK_STONE;
				}
			}
			else
			{
				unsigned char surfaceBlock = 0;
				double vegetationLevel = vegetationGenerator.get(chunkPosition.x * 16 + x, chunkPosition.z * 16 + z);
				double coarsenessLevel = coarsenessGenerator.get(chunkPosition.x * 16 + x, chunkPosition.z * 16 + z);

				if (vegetationLevel < 0.15)
				{
					if (coarsenessLevel < 0.7)
					{
						surfaceBlock = BLOCK_DIRT;
					}
					else
					{
						surfaceBlock = BLOCK_GRAVEL;
					}
				}
				else
				{
					if (coarsenessLevel < 0.8)
					{
						surfaceBlock = BLOCK_GRASS;
					}
					else
					{
						surfaceBlock = BLOCK_PODZOL;
					}
				}

				for (int y = height - 4; y <= height; y++)
				{
					if (blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] == BLOCK_AIR) continue;
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = y == height ? surfaceBlock : BLOCK_DIRT;
				}
			}
		}
	}

	return {};
}