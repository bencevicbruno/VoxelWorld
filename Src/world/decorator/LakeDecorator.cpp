#include "world/decorator/LakeDecorator.h"

#include "world/chunk/Chunk.h"

LakeDecorator::LakeDecorator(int lakeLevel) :
	lakeLevel(lakeLevel)
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> LakeDecorator::decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			for (int y = heightMap[x * CHUNK_WIDTH + z]; y <= lakeLevel; y++)
			{
				blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = y == lakeLevel ? BLOCK_WATER_SURFACE : BLOCK_WATER;
			}
		}
	}

	return {};
}