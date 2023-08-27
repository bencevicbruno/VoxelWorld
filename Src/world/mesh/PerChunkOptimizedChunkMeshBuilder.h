#pragma once

#include "world/mesh/ChunkMeshBuilder.h"
#include "world/Chunk.h"

class PerChunkOptimizedChunkMeshBuilder : public ChunkMeshBuilder
{
public:
	static PerChunkOptimizedChunkMeshBuilder& GetInstance();

	virtual ~PerChunkOptimizedChunkMeshBuilder() = default;

	virtual ChunkMesh generateMesh(Chunk* chunk) const;

private:
	bool shouldRenderSide(const Block& block, const Block& neighbouringBlock) const;
};