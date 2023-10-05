#include "world/decorator/SurfaceVegetationDecorator.h"

#include "world/chunk/Chunk.h"

SurfaceVegetationDecorator::SurfaceVegetationDecorator(int seed) :
	seed(seed)
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>
	SurfaceVegetationDecorator::decorate(const Vector & chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int height = heightMap[x * CHUNK_WIDTH + z];
			unsigned char surfaceBlock = blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];

			unsigned char blockAbove;
			if ((height + 1) >= CHUNK_HEIGHT)
				blockAbove = BLOCK_AIR;
			else
				blockAbove = blocks[(height + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];

			if (surfaceBlock == BLOCK_AIR) continue;
			if (blockAbove != BLOCK_AIR) continue;
			if ((float(std::rand()) / RAND_MAX) > 0.5) continue;

			switch (surfaceBlock)
			{
			case BLOCK_GRASS:
				addGrassVegetation(x, z, blocks, height);
				break;
			case BLOCK_DIRT:
				addDirtVegetation(x, z, blocks, height, false);
				break;
			case BLOCK_PODZOL:
				addDirtVegetation(x, z, blocks, height, true);
				break;
			}
		}
	}

	return {};
}


void SurfaceVegetationDecorator::addGrassVegetation(int x, int z, unsigned char* blocks, int height) const
{
	bool isFlower = (float(std::rand()) / RAND_MAX) > 0.75;
	bool isSpecial = (float(std::rand()) / RAND_MAX) > 0.65;

	unsigned char decoBlock = 0;

	if (isFlower) {
		decoBlock = isSpecial ? BLOCK_FLOWER_RED : BLOCK_FLOWER_YELLOW;
	}
	else {
		decoBlock = isSpecial ? BLOCK_HUGE_GRASS : BLOCK_TALL_GRASS;
	}

	if ((height + 1) >= CHUNK_HEIGHT) return;
	blocks[(height + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = decoBlock;
	if (decoBlock == BLOCK_HUGE_GRASS)
	{
		if ((height + 2) >= CHUNK_HEIGHT) return;
		blocks[(height + 2) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_HUGE_GRASS_TOP;
	}
}

void SurfaceVegetationDecorator::addDirtVegetation(int x, int z, unsigned char* blocks, int height, bool allowMushrooms) const
{
	bool isMushroom = (float(std::rand()) / RAND_MAX) > (allowMushrooms ? 0.75 : 0.99);
	bool isSpecial = (float(std::rand()) / RAND_MAX) > 0.65;;

	unsigned char decoBlock = 0;

	if (isMushroom && allowMushrooms) {
		decoBlock = isSpecial ? BLOCK_MUSHROOM_RED : BLOCK_MUSHROOM_BROWN;
	}
	else {
		decoBlock = isSpecial ? BLOCK_DEAD_LEAVES : BLOCK_TALL_GRASS;
	}

	if ((height + 1) >= CHUNK_HEIGHT) return;
	blocks[(height + 1) * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = decoBlock;
}
