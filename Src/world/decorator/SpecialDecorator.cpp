#include "world/decorator/SpecialDecorator.h"

#include "world/chunk/Chunk.h"

SpecialDecorator::SpecialDecorator(int lakeLevel) :
	lakeLevel(lakeLevel)
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> SpecialDecorator::decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	int height = heightMap[11 * CHUNK_WIDTH + 11];

	if (height <= lakeLevel) return {};

	if (blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + 11 * CHUNK_WIDTH + 11] != BLOCK_AIR && height < (CHUNK_HEIGHT - 2))
	{
		blocks[(height + 1) * CHUNK_WIDTH * CHUNK_WIDTH + 11 * CHUNK_WIDTH + 11] = BLOCK_SPEACIAL_FLOWER;
	}

	return {};
}