#include "world/decorator/decoration/OakTreeWorldDecoration.h"

#include "utils/debug/Debug.h"

std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> OakTreeWorldDecoration::getBlocks(const Vector& startPosition) const
{
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> blocks;

	// 32 is seaLevel
	if (startPosition.y <= 32)
	{
		return blocks;
	}

	for (int y = startPosition.y; y <= startPosition.y + 5; y++)
	{
		addBlock(blocks, { startPosition.x, float(y), startPosition.z }, BLOCK_OAK_LOG);
	}

	for (int dx = -2; dx <= 2; dx++)
	{
		for (int dz = -2; dz <= 2; dz++)
		{
			if (dx == 0 && dz == 0) continue;

			addBlock(blocks, { startPosition.x + dx, startPosition.y + 4, startPosition.z + dz }, BLOCK_OAK_LEAVES);
			addBlock(blocks, { startPosition.x + dx, startPosition.y + 5, startPosition.z + dz }, BLOCK_OAK_LEAVES);
		}
	}

	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dz = -1; dz <= 1; dz++)
		{
			addBlock(blocks, { startPosition.x + dx, startPosition.y + 6, startPosition.z + dz }, BLOCK_OAK_LEAVES);
		}
	}

	addBlock(blocks, { startPosition.x, startPosition.y + 7, startPosition.z }, BLOCK_OAK_LEAVES);
	//Debug::printBlocksInChunks(blocks);
	return blocks;
}