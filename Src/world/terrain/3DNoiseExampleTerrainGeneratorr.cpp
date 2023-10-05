#include "3DNoiseExampleTerrainGeneratorr.h"

#include "math/PerlinNoiseGenerator.h"
#include "world/chunk/Chunk.h"

unsigned char* ThreeDNoiseExampleTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const
{
	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	PerlinNoiseGenerator noiseGenerator(123, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 4);

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = position.x * 16 + x;

		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = position.z * 16 + z;

			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				double value = noiseGenerator.get(globalX, y, globalZ);

				bool useType1 = true;

				if (useType1)
				{
					blocks[Chunk::coordsToOffset(x, y, z)] = value > 0.5;
				}
				else
				{
					blocks[Chunk::coordsToOffset(x, y, z)] = value >= 0.4 && value <= 0.6;
				}
			}
		}
	}

	return blocks;
}

int* ThreeDNoiseExampleTerrainGenerator::generateHeightMap(int x, int z)
{
	return nullptr;
}