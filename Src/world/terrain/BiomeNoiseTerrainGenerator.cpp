#include "world/terrain/BiomeNoiseTerrainGenerator.h"

#include "world/Blocks.h"
#include "world/chunk/Chunk.h"

BiomeNoiseTerrainGenerator::BiomeNoiseTerrainGenerator(unsigned int seed) :
	TerrainGenerator(),
	noiseGenerator(PerlinNoiseGenerator(seed, 0.01))
{}

unsigned char* BiomeNoiseTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const
{
	PerlinNoiseGenerator decorationsNoiseGenerator(seed * 0.1, 1);

	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalX = 16 * position.x + x;
			int globalZ = 16 * position.z + z;

			double value = noiseGenerator.get(globalX, globalZ);
			int coordinates = 0 * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z;
			//std::cout << value << std::endl;
			if (value >= 0.5 && value <= 0.6) {
				blocks[coordinates] = BLOCK_SAND;
			}
			else {
				blocks[coordinates] = BLOCK_STONE;
			}

			if (decorationsNoiseGenerator.get(globalX, globalZ) > 0.7 && (((x + z) % 11) == 0))
			{
				blocks[coordinates] = BLOCK_GRASS;
			}
		}
	}

	return blocks;
}

int BiomeNoiseTerrainGenerator::getHeight(int x, int z) const
{
	return 0;
}

void BiomeNoiseTerrainGenerator::spawnTree(int x, int z)
{
	for (int y = 1; y <= 5; y++)
	{
	}
}