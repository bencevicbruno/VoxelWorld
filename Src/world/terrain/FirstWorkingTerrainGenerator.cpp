#include "world/terrain/FirstWorkingTerrainGenerator.h"

#include "world/Blocks.h"
#include "world/chunk/Chunk.h"

FirstWorkingTerrainGenerator::FirstWorkingTerrainGenerator(unsigned int seed) :
	TerrainGenerator(),
	heightGenerator(OctaveNoiseGenerator(seed, 8.0, 0.05, 0.01)),
	heightModifierGenerator(OctaveNoiseGenerator(seed, 8.0, 0.5, 0.01)),
	caveGenerator(PerlinNoiseGenerator(seed, 0.01)),
	coverGenerator(PerlinNoiseGenerator(seed, 0.005))
{}

unsigned char* FirstWorkingTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position, const std::unordered_map<Vector, unsigned char>& pendingBlocks) const
{
	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	int seaLevel = 32;
	int snowLevel = 50;

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = 16 * position.x + x;
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = 16 * position.z + z;

			int height = getHeight(position.x + x, position.z + z);
			unsigned char coverBlock = coverGenerator.get(globalX, globalZ) > 0.5 ? BLOCK_GRASS : BLOCK_SAND;

			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int globalY = y + position.y;

				if (globalY > snowLevel && globalY <= height)
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = coverBlock == BLOCK_SAND ? BLOCK_STONE : BLOCK_SNOW;
				}
				else if (globalY == height)
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = (globalY < seaLevel&& coverBlock == BLOCK_GRASS) ? BLOCK_DIRT : coverBlock;
				}
				else if (globalY >= (height - 4) && globalY <= (height - 1))
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = coverBlock == BLOCK_SAND ? BLOCK_SAND : BLOCK_DIRT;
				}
				else if (globalY < (height - 4)) {
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_STONE;
				}
				else
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_AIR;
				}

				if ((globalY <= (seaLevel - 1)) && (blocks[x * 16 * 16 + z * 16 + y] == BLOCK_AIR)) {
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_WATER;
				}
				else if ((globalY == seaLevel) && (blocks[x * 16 * 16 + z * 16 + y] == BLOCK_AIR)) {
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_WATER_SURFACE;
				}

				double caveValue = caveGenerator.get(x, y, z);
				if (caveValue < 0.5)
				{
					blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_AIR;
				}
			}
		}
	}

	std::cout << pendingBlocks.size() << std::endl;
	for (auto& [position, blockID] : pendingBlocks)
	{
		blocks[int(position.y * CHUNK_WIDTH * CHUNK_WIDTH + position.x * CHUNK_WIDTH + position.z)] = blockID;
	}

	return blocks;
}

int FirstWorkingTerrainGenerator::getHeight(int x, int z) const
{
	int height = 8 + int(heightGenerator.get01(double(x), double(z)) * (64 - 8));
	height += heightModifierGenerator.get01(double(x), double(z)) * 16;

	return height;
}