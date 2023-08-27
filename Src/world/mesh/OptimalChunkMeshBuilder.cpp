#include "world/mesh/OptimalChunkMeshBuilder.h"

#include "world/utils/BlockRegistry.h"
#include "world/Chunk.h"
#include "world/World.h"

#include "Options.h"

OptimalChunkMeshBuilder& OptimalChunkMeshBuilder::GetInstance()
{
	static OptimalChunkMeshBuilder builder;

	return builder;
}

ChunkMesh OptimalChunkMeshBuilder::generateMesh(Chunk* chunk) const
{
	ChunkMesh chunkMesh;

	generateMiddle(chunk, chunkMesh);

	generateNorth(chunk, chunkMesh);
	generateSouth(chunk, chunkMesh);
	generateWest(chunk, chunkMesh);
	generateEast(chunk, chunkMesh);

	return chunkMesh;
}

bool OptimalChunkMeshBuilder::shouldRenderSide(const Block& block, const Block& neighbouringBlock) const
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

void OptimalChunkMeshBuilder::generateMiddle(Chunk* chunk, ChunkMesh& chunkMesh) const
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			for (int z = 0; z < CHUNK_WIDTH; z++)
			{
				unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];
				if (currentBlockID == 0) continue;

				Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
				Mesh& mesh = chunkMesh.getMeshForBlockID(currentBlockID);

				// BOTTOM FACE
				if (auto blockID = chunk->getBlockAt(x, y - 1, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)) && !(y == 0 && !ENABLE_CHUNK_EDGES))
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
				if (z != 0)
				{
					if (auto blockID = chunk->getBlockAt(x, y, z - 1))
					{
						if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
						{
							mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, z }));
						}
					}
				}

				// SOUTH FACE
				if (z != (CHUNK_WIDTH - 1))
				{
					if (auto blockID = chunk->getBlockAt(x, y, z + 1))
					{
						if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
						{
							mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, z }));
						}
					}
				}

				// WEST FACE
				if (x != 0)
				{
					if (auto blockID = chunk->getBlockAt(x - 1, y, z))
					{
						if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
						{
							mesh.addMesh(currentBlock.mesh.getWestFace({ x, y, z }));
						}
					}
				}

				// EAST FACE
				if (x != (CHUNK_WIDTH - 1))
				{
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
	}
}

void OptimalChunkMeshBuilder::generateNorth(Chunk* chunk, ChunkMesh& chunkMesh) const
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	Chunk* chunkNorth = chunk->world->getChunkAt(chunk->getPosition().goNorth(16));

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + 0];
			if (currentBlockID == 0) continue;

			Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
			Mesh& mesh = chunkMesh.getMeshForBlockID(currentBlockID);

			if (chunkNorth == nullptr)
			{
				if (ENABLE_CHUNK_EDGES)
					mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, 0 }));
			} 
			else {
				if (auto blockID = chunkNorth->getBlockAt(x, y, 15))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, 0 }));
					}
				}
			}
		}
	}
}

void OptimalChunkMeshBuilder::generateSouth(Chunk* chunk, ChunkMesh& chunkMesh) const
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	Chunk* chunkSouth = chunk->world->getChunkAt(chunk->getPosition().goSouth(16));

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + 15];
			if (currentBlockID == 0) continue;

			Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
			Mesh& mesh = chunkMesh.getMeshForBlockID(currentBlockID);

			if (chunkSouth == nullptr)
			{
				if (ENABLE_CHUNK_EDGES)
					mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, 15 }));
			}
			else
			{
				if (auto blockID = chunkSouth->getBlockAt(x, y, 0))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, 15 }));
					}
				}
			}
		}
	}
}

void OptimalChunkMeshBuilder::generateWest(Chunk* chunk, ChunkMesh& chunkMesh) const
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	Chunk* chunkWest = chunk->world->getChunkAt(chunk->getPosition().goWest(16));

	for (int z = 0; z < CHUNK_WIDTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + 0 * CHUNK_WIDTH + z];
			if (currentBlockID == 0) continue;

			Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
			Mesh& mesh = chunkMesh.getMeshForBlockID(currentBlockID);

			if (chunkWest == nullptr)
			{
				if (ENABLE_CHUNK_EDGES)
					mesh.addMesh(currentBlock.mesh.getWestFace({ 0, y, z }));
			}
			else
			{
				if (auto blockID = chunkWest->getBlockAt(15, y, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getWestFace({ 0, y, z }));
					}
				}
			}
		}
	}
}

void OptimalChunkMeshBuilder::generateEast(Chunk* chunk, ChunkMesh& chunkMesh) const
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	Chunk* chunkEast = chunk->world->getChunkAt(chunk->getPosition().goEast(16));

	for (int z = 0; z < CHUNK_WIDTH; z++)
	{
		for (int y = 0; y < CHUNK_HEIGHT; y++)
		{
			unsigned char currentBlockID = chunk->blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + 15 * CHUNK_WIDTH + z];
			if (currentBlockID == 0) continue;

			Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
			Mesh& mesh = chunkMesh.getMeshForBlockID(currentBlockID);

			if (chunkEast == nullptr)
			{
				if (ENABLE_CHUNK_EDGES)
					mesh.addMesh(currentBlock.mesh.getEastFace({ 15, y, z }));
			}
			else
			{
				if (auto blockID = chunkEast->getBlockAt(0, y, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getEastFace({ 15, y, z }));
					}
				}
			}
		}
	}
}