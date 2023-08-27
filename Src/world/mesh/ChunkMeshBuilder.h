#pragma once

#include "world/ChunkMesh.h"

class Chunk;

class ChunkMeshBuilder
{
public:
	virtual ChunkMesh generateMesh(Chunk* chunk) const = 0;
	virtual ~ChunkMeshBuilder() = default;
};