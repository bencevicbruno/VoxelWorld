#pragma once

#include "world/mesh/ChunkMeshBuilder.h"
#include "world/chunk/Chunk.h"

class OptimalChunkMeshBuilder : public ChunkMeshBuilder
{
public:
	virtual ~OptimalChunkMeshBuilder() = default;

	virtual ChunkMesh* generateMesh(Chunk* chunk) const;

private:
	bool shouldRenderSide(const Block& block, const Block& neighbouringBlock) const;

	void generateMiddle(Chunk* chunk, ChunkMesh& chunkMesh) const;
	void generateNorth(Chunk* chunk, ChunkMesh& chunkMesh) const;
	void generateSouth(Chunk* chunk, ChunkMesh& chunkMesh) const;
	void generateWest(Chunk* chunk, ChunkMesh& chunkMesh) const;
	void generateEast(Chunk* chunk, ChunkMesh& chunkMesh) const;
};