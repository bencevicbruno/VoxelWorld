#include "world/mesh/PerChunkOptimizedChunkMeshBuilder.h"

#include "world/utils/BlockRegistry.h"
#include "world/chunk/Chunk.h"

ChunkMesh* PerChunkOptimizedChunkMeshBuilder::generateMesh(Chunk* chunk) const
{
	//std::cout << "Gen for " << chunk->getPosition() << std::endl;
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
	ChunkMesh* chunkMesh = new ChunkMesh();

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];
				if (currentBlockID == 0) continue;

				Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
				Mesh& mesh = chunkMesh->getMeshForBlockID(currentBlockID);

				if (currentBlockID == BLOCK_TALL_GRASS || currentBlockID == BLOCK_FLOWER_YELLOW || currentBlockID == BLOCK_FLOWER_RED)
				{
					mesh.addMesh(currentBlock.mesh.getCrossMesh({ x, y, z }));
					continue;
				}

				// BOTTOM FACE
				if (auto blockID = chunk->getBlockAt(x, y - 1, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getBottomFace({ x, y, z }));
					}
				}

				// TOP FACE
				if (auto blockID = chunk->getBlockAt(x, y + 1, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
					}
				}

				// NORTH FACE
				if (auto blockID = chunk->getBlockAt(x, y, z - 1))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, z }));
					}
				}

				// SOUTH FACE
				if (auto blockID = chunk->getBlockAt(x, y, z + 1))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, z }));
					}
				}

				// WEST FACE
				if (auto blockID = chunk->getBlockAt(x - 1, y, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getWestFace({ x, y, z }));
					}
				}

				// EAST FACE
				if (auto blockID = chunk->getBlockAt(x + 1, y, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getEastFace({ x, y, z }));
					}
				}
			}
		}
	}

	return chunkMesh;
}

bool PerChunkOptimizedChunkMeshBuilder::shouldRenderSide(const Block& block, const Block& neighbouringBlock) const
{
	switch (block.opacity)
	{
	case BlockOpacity::OPAQUE:
		return neighbouringBlock.opacity != BlockOpacity::OPAQUE;
	case BlockOpacity::SEE_THROUGH:
		return (block != neighbouringBlock) && ((neighbouringBlock.opacity == BlockOpacity::TRANSPARENT) || (neighbouringBlock.opacity == BlockOpacity::EMPTY));
	case BlockOpacity::TRANSPARENT:
		return (block != neighbouringBlock) && (neighbouringBlock.opacity == BlockOpacity::EMPTY);
	default:
		return false;
	}
}
