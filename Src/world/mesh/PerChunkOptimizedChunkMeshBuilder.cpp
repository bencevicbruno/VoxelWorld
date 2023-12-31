#include "world/mesh/PerChunkOptimizedChunkMeshBuilder.h"

#include "world/utils/BlockRegistry.h"
#include "world/chunk/Chunk.h"

ChunkMesh* PerChunkOptimizedChunkMeshBuilder::generateMesh(Chunk* chunk) const
{
	//std::cout << "Gen for " << chunk->getPosition() << std::endl;
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();
	ChunkMesh* chunkMesh = new ChunkMesh();

	Block& waterSurfaceBlock = blockRegistry.getBlockByID(BLOCK_WATER_SURFACE);
	Mesh& waterMesh = chunkMesh->getMeshForBlockID(BLOCK_WATER);

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				unsigned char currentBlockID = chunk->blocks[Chunk::coordsToOffset(x, y, z)];
				if (currentBlockID == 0) continue;

				Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
				Mesh& mesh = chunkMesh->getMeshForBlockID(currentBlockID);

				bool isSedge = currentBlockID == BLOCK_SEDGE || currentBlockID == BLOCK_SEDGE_TOP;

				if (currentBlockID == BLOCK_TALL_GRASS
					|| currentBlockID == BLOCK_FLOWER_YELLOW
					|| currentBlockID == BLOCK_FLOWER_RED
					|| currentBlockID == BLOCK_HUGE_GRASS
					|| currentBlockID == BLOCK_HUGE_GRASS_TOP
					|| currentBlockID == BLOCK_MUSHROOM_BROWN
					|| currentBlockID == BLOCK_MUSHROOM_RED
					|| currentBlockID == BLOCK_SPEACIAL_FLOWER)
				{
					mesh.addMesh(currentBlock.mesh.getCrossMesh({ x, y, z }));
					continue;
				}

				if (currentBlockID == BLOCK_LILY_PAD
					|| currentBlockID == BLOCK_LILY_PADS)
				{
					mesh.addMesh(currentBlock.mesh.getBottomFace({ x, y, z }));
					mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
					continue;
				}

				if (currentBlockID == BLOCK_DEAD_LEAVES)
				{
					mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
					continue;
				}

				if (isSedge)
				{
					mesh.addMesh(currentBlock.mesh.getCrossMesh({ x, y, z }));
				}

				// BOTTOM FACE
				if (auto blockID = chunk->getBlockAt(x, y - 1, z))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getBottomFace({ x, y, z }));
					}
				}

				// TOP FACE
				if (auto blockID = chunk->getBlockAt(x, y + 1, z))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
					}

					if (currentBlockID == BLOCK_SEDGE)
					{
						if (shouldRenderSide(waterSurfaceBlock, blockRegistry.getBlockByID(*blockID)))
						{
							waterMesh.addMesh(waterSurfaceBlock.mesh.getTopFace({ x, y, z }));
						}
					}
				}

				// NORTH FACE
				if (auto blockID = chunk->getBlockAt(x, y, z - 1))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, z }));
					}
				}

				// SOUTH FACE
				if (auto blockID = chunk->getBlockAt(x, y, z + 1))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, z }));
					}
				}

				// WEST FACE
				if (auto blockID = chunk->getBlockAt(x - 1, y, z))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getWestFace({ x, y, z }));
					}
				}

				// EAST FACE
				if (auto blockID = chunk->getBlockAt(x + 1, y, z))
				{
					if (!isSedge && shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
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
	if (block.id == BLOCK_AIR) return false;

	if (neighbouringBlock.id == BLOCK_AIR) return true;

	if ((block.id == BLOCK_WATER_SURFACE || block.id == BLOCK_WATER) && neighbouringBlock.id == BLOCK_SEDGE) return false;

	switch (block.opacity)
	{
	case BlockOpacity::OPAQUE:
		return neighbouringBlock.opacity != BlockOpacity::OPAQUE;
	case BlockOpacity::SEE_THROUGH:
		return (block != neighbouringBlock) && (neighbouringBlock.opacity != BlockOpacity::OPAQUE);
	case BlockOpacity::TRANSPARENT:
		return (block != neighbouringBlock) && (neighbouringBlock.opacity == BlockOpacity::EMPTY || neighbouringBlock.opacity == BlockOpacity::SEE_THROUGH);
	default:
		return false;
	}
}