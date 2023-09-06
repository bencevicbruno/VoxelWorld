#include "world/decorator/ForestDecorator.h"

#include "world/chunk/Chunk.h"

ForestDecorator::ForestDecorator(int seed, int lakeLevel):
	seed(seed),
	lakeLevel(lakeLevel),
	placementNoiseGenerator(PerlinNoiseGenerator(seed << 10, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 25)),
	sizeNoiseGenerator(PerlinNoiseGenerator(seed << 5, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 18))
{}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> ForestDecorator::decorate(const Vector & chunkPosition, unsigned char* blocks, const int* heightMap) const
{
	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = chunkPosition.x * 16 + x;

 		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = chunkPosition.z * 16 + z;
			int height = heightMap[x * CHUNK_WIDTH + z];

			// Check if it's underwater or on beach
			if (height <= (lakeLevel + 2)) continue;

			// Check if the surface is grass or dirt
			if (blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] != BLOCK_GRASS
				&& blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] != BLOCK_DIRT
				&& blocks[height * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] != BLOCK_PODZOL) continue;

			// Check if a decoration placement should take place on these coordinates
			double placementValue = placementNoiseGenerator.get(globalX, globalZ);
			if (placementValue < 0.9) continue;

			double sizeValue = sizeNoiseGenerator.get(globalX, height, globalZ);

			if (sizeValue > 0.8 && sizeValue < 0.9)
			{
				createSmallTree({ x, height + 1, z }, chunkPosition, blocks);
			}
			else if (sizeValue > 0.9)
			{
				createBigTree({ x, height + 1, z }, chunkPosition, blocks);
			}
		}
	}

	return {};
}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> ForestDecorator::createSmallTree(const Vector& startPosition, const Vector& chunkPosition, unsigned char* blocks) const
{
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> leftovers;

	int extraHeight = rand() % 3;
	int treeHeight = 6 + extraHeight;

	for (int y = startPosition.y; y <= startPosition.y + treeHeight; y++)
	{
		int x = startPosition.x;
		int z = startPosition.z;

		if (y >= CHUNK_HEIGHT) continue;
		blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_OAK_LOG;
	}

	int treeRadius = 3;
	int layerCounter = 0;
	for (int y = startPosition.y + 3 + extraHeight; y <= startPosition.y + treeHeight + 3 + extraHeight; y++)
	{
		if (y >= CHUNK_HEIGHT) continue;

		for (int x = chunkPosition.x * 16 + startPosition.x - treeRadius; x <= chunkPosition.x * 16 + startPosition.x + treeRadius; x++)
		{
			for (int z = chunkPosition.z * 16 + startPosition.z - treeRadius; z <= chunkPosition.z * 16 + startPosition.z + treeRadius; z++)
			{
				Vector localPos = Vector::GlobalToLocal({ x, y, z });
				Vector chunkPos = Vector::GlobalToChunk({ x, y, z });

				if (chunkPos == chunkPosition)
				{
					if (blocks[int(localPos.y * CHUNK_WIDTH * CHUNK_WIDTH + localPos.x * CHUNK_WIDTH + localPos.z)] != BLOCK_AIR) continue;
					blocks[int(localPos.y * CHUNK_WIDTH * CHUNK_WIDTH + localPos.x * CHUNK_WIDTH + localPos.z)] = BLOCK_OAK_LEAVES;
				}
				else
				{
					leftovers[chunkPos][localPos] = BLOCK_OAK_LEAVES;
				}
			}
		}

		if (layerCounter == 2)
		{
			layerCounter = 0;
			treeRadius -= 1;
		}

		layerCounter++;
	}

	return leftovers;
}

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> ForestDecorator::createBigTree(const Vector& startPosition, const Vector& chunkPosition, unsigned char* blocks) const
{
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> leftovers;

	int extraHeight = rand() % 3;
	int treeHeight = 10 + extraHeight;

	for (int y = startPosition.y; y <= startPosition.y + treeHeight; y++)
	{
		int x = startPosition.x;
		int z = startPosition.z;

		if (y >= CHUNK_HEIGHT) continue;
		blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z] = BLOCK_OAK_LOG;
	}

	int treeRadius = 5;
	int layerCounter = 0;
	for (int y = startPosition.y + 3 + extraHeight; y <= startPosition.y + treeHeight + 3 + extraHeight; y++)
	{
		if (y >= CHUNK_HEIGHT) continue;

		for (int x = chunkPosition.x * 16 + startPosition.x - treeRadius; x <= chunkPosition.x * 16 + startPosition.x + treeRadius; x++)
		{
			for (int z = chunkPosition.z * 16 + startPosition.z - treeRadius; z <= chunkPosition.z * 16 + startPosition.z + treeRadius; z++)
			{
				Vector localPos = Vector::GlobalToLocal({ x, y, z });
				Vector chunkPos = Vector::GlobalToChunk({ x, y, z });

				if (chunkPos == chunkPosition)
				{
					if (blocks[int(localPos.y * CHUNK_WIDTH * CHUNK_WIDTH + localPos.x * CHUNK_WIDTH + localPos.z)] != BLOCK_AIR) continue;
					blocks[int(localPos.y * CHUNK_WIDTH * CHUNK_WIDTH + localPos.x * CHUNK_WIDTH + localPos.z)] = BLOCK_OAK_LEAVES;
				}
				else
				{
					leftovers[chunkPos][localPos] = BLOCK_OAK_LEAVES;
				}
			}
		}

		if (layerCounter == 2)
		{
			layerCounter = 0;
			treeRadius -= 1;
		}

		layerCounter++;
	}

	return leftovers;
}
