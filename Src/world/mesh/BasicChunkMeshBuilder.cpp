#include "world/mesh/BasicChunkMeshBuilder.h"

#include "world/utils/BlockRegistry.h"
#include "world/chunk/Chunk.h"

ChunkMesh* BasicChunkMeshBuilder::generateMesh(Chunk* chunk) const
{
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
				Mesh& waterMesh = chunkMesh->getMeshForBlockID(BLOCK_WATER);

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

				if (currentBlockID == BLOCK_SEDGE
					|| currentBlockID == BLOCK_SEDGE_TOP)
				{
					Block& waterBlock = blockRegistry.getBlockByID(BLOCK_WATER_SURFACE);
					waterMesh.addMesh(waterBlock.mesh.getBottomFace({ x, y, z }));
					waterMesh.addMesh(waterBlock.mesh.getTopFace({ x, y, z }));
					waterMesh.addMesh(waterBlock.mesh.getNorthFace({ x, y, z }));
					waterMesh.addMesh(waterBlock.mesh.getSouthFace({ x, y, z }));
					waterMesh.addMesh(waterBlock.mesh.getWestFace({ x, y, z }));
					waterMesh.addMesh(waterBlock.mesh.getEastFace({ x, y, z }));
				}

				mesh.addMesh(currentBlock.mesh.getBottomFace({ x, y, z }));
				mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
				mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, z }));
				mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, z }));
				mesh.addMesh(currentBlock.mesh.getWestFace({ x, y, z }));
				mesh.addMesh(currentBlock.mesh.getEastFace({ x, y, z }));
			}
		}
	}

	return chunkMesh;
}