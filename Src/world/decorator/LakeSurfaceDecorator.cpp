#include "world/decorator/LakeSurfaceDecorator.h"

#include "world/chunk/Chunk.h"

LakeSurfaceDecorator::LakeSurfaceDecorator(int lakeLevel) :
	lakeLevel(lakeLevel)
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> LakeSurfaceDecorator::decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			unsigned char surfaceBlock = blocks[lakeLevel * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];
			unsigned char blockBelow = blocks[(lakeLevel - 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];

			if (!(surfaceBlock == BLOCK_WATER_SURFACE && blockBelow != BLOCK_WATER)) continue;

			blocks[lakeLevel * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_SEDGE;
			blocks[(lakeLevel + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_SEDGE_TOP;
		}
	}

	return {};
}