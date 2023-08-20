#include "world/terrain/BiomeNoiseTerrainGenerator.h"

#include "world/Blocks.h"
#include "world/Chunk.h"

BiomeNoiseTerrainGenerator::BiomeNoiseTerrainGenerator(unsigned int seed) :
	TerrainGenerator(),
	noiseGenerator(PerlinNoiseGenerator(seed, 0.01))
{}

unsigned char* BiomeNoiseTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position) const
{
	if (position.y >= 16)
	{
		return (unsigned char*) calloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE, sizeof(unsigned char));
	}

	unsigned char* blocks = new unsigned char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int globalX = position.x + x;
				int globalZ = position.z + z;

				double value = noiseGenerator.get(globalX, globalZ);
				int coordinates = x * 16 * 16 + z * 16 + y;

				if (value >= 0.5 && value <= 0.6) {
					blocks[coordinates] = BLOCK_WATER;
				}
				else {
					blocks[coordinates] = BLOCK_GRASS;
				}
			}
		}
	}

	return blocks;
}