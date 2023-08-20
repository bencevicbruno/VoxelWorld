#include "world/terrain/FirstWorkingTerrainGenerator.h"

#include "world/Blocks.h"
#include "world/Chunk.h"

FirstWorkingTerrainGenerator::FirstWorkingTerrainGenerator(unsigned int seed) :
	TerrainGenerator(),
	heightGenerator(OctaveNoiseGenerator(seed, 8.0, 0.05, 0.01)),
	heightModifierGenerator(OctaveNoiseGenerator(seed, 8.0, 0.5, 0.01)),
	caveGenerator(PerlinNoiseGenerator(seed, 0.01)),
	coverGenerator(PerlinNoiseGenerator(seed, 0.005))
{}

unsigned char* FirstWorkingTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position) const
{
	unsigned char* blocks = new unsigned char[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];

	int seaLevel = 32;
	int snowLevel = 50;

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int z = 0; z < CHUNK_SIZE; z++)
		{
			int height = 8 + int(heightGenerator.get01(double(position.x + x), double(position.z + z)) * (64 - 8));
			height += heightModifierGenerator.get01(double(position.x + x), double(position.z + z)) * 16;
			unsigned char coverBlock = coverGenerator.get01(position.x + x, position.z + z) > 0.5 ? BLOCK_GRASS : BLOCK_SAND;

			for (int y = 0; y < CHUNK_SIZE; y++)
			{
				int globalY = y + position.y;

				if (globalY > snowLevel && globalY <= height)
				{
					blocks[x * 16 * 16 + z * 16 + y] = coverBlock == BLOCK_SAND ? BLOCK_STONE : BLOCK_SNOW;
				}
				else if (globalY == height)
				{
					blocks[x * 16 * 16 + z * 16 + y] = (globalY < seaLevel&& coverBlock == BLOCK_GRASS) ? BLOCK_DIRT : coverBlock;
				}
				else if (globalY >= (height - 4) && globalY <= (height - 1))
				{
					blocks[x * 16 * 16 + z * 16 + y] = coverBlock == BLOCK_SAND ? BLOCK_SAND : BLOCK_DIRT;
				}
				else if (globalY < (height - 4)) {
					blocks[x * 16 * 16 + z * 16 + y] = BLOCK_STONE;
				}
				else
				{
					blocks[x * 16 * 16 + z * 16 + y] = BLOCK_AIR;
				}

				if ((globalY <= (seaLevel - 1)) && (blocks[x * 16 * 16 + z * 16 + y] == BLOCK_AIR)) {
					blocks[x * 16 * 16 + z * 16 + y] = BLOCK_WATER;
				}
				else if ((globalY == seaLevel) && (blocks[x * 16 * 16 + z * 16 + y] == BLOCK_AIR)) {
					blocks[x * 16 * 16 + z * 16 + y] = BLOCK_WATER_SURFACE;
				}

				double caveValue = caveGenerator.get01(x, y, z);
				if (caveValue < 0.5)
				{
					blocks[x * 16 * 16 + z * 16 + y] = BLOCK_AIR;
				}
			}
		}
	}

	return blocks;
}