#pragma once

#include "world/mesh/ChunkMeshBuilder.h"
#include "world/Chunk.h"

class BasicChunkMeshBuilder : public ChunkMeshBuilder
{
public:
	static BasicChunkMeshBuilder& GetInstance();

	virtual ~BasicChunkMeshBuilder() = default;

	virtual ChunkMesh generateMesh(Chunk* chunk) const;
private:
};