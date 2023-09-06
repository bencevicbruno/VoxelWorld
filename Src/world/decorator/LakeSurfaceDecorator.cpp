#include "world/decorator/LakeSurfaceDecorator.h"

#include "math/Math.h"
#include "world/chunk/Chunk.h"

LakeSurfaceDecorator::LakeSurfaceDecorator(int lakeLevel) :
	lakeLevel(lakeLevel),
	lilyPadsNoiseGenerator(PerlinNoiseGenerator(0, 12))
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> LakeSurfaceDecorator::decorate(const Vector& chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int height = heightMap[x * CHUNK_WIDTH + z];

			if (blocks[lakeLevel * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] != BLOCK_WATER_SURFACE) continue;

			if ((std::abs(x + z) % 7) > 3)
			{
				placeSedges(x, z, blocks, heightMap);
			}

			if (lilyPadsNoiseGenerator.get(chunkPosition.x * 16 + x, chunkPosition.z * 16 + z) < 0.4)
			{
				placeLilyPads(x, z, blocks, heightMap);
			}
		}
	}

	return {};
}

void LakeSurfaceDecorator::placeSedges(int x, int z, unsigned char* blocks, const int* heightMap) const
{
	unsigned char blockBelow = blocks[(lakeLevel - 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];
	unsigned char blockBelow2 = blocks[(lakeLevel - 2) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];

	bool isOneBlockDeepWater = blockBelow != BLOCK_WATER;
	bool isTwoBlockDeepWater = blockBelow == BLOCK_WATER && blockBelow2 != BLOCK_WATER;

	if (!(isOneBlockDeepWater || isTwoBlockDeepWater)) return;

	blocks[lakeLevel * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_SEDGE;
	blocks[(lakeLevel + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_SEDGE_TOP;
}

void LakeSurfaceDecorator::placeLilyPads(int x, int z, unsigned char* blocks, const int* heightMap) const
{
	unsigned char blockAboveWater = blocks[(lakeLevel + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];

	if (blockAboveWater != BLOCK_AIR) return;

	blocks[(lakeLevel + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = (std::abs(x + z) % 10 < 5) ? BLOCK_LILY_PAD : BLOCK_LILY_PADS;
}