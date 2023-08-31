#pragma once

#include "world/mesh/ChunkMeshBuilder.h"
#include "world/chunk/Chunk.h"

class BasicChunkMeshBuilder : public ChunkMeshBuilder
{
public:
	virtual ~BasicChunkMeshBuilder() = default;

	virtual ChunkMesh* generateMesh(Chunk* chunk) const;
private:
};