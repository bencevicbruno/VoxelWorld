#pragma once

#include "renderer/models/Mesh.h"

struct ChunkMesh
{
	Mesh solidMesh;
	Mesh transparentMesh;
	Mesh waterMesh;

	ChunkMesh() = default;
	ChunkMesh(Mesh&& solidMesh, Mesh&& transparentMesh, Mesh&& waterMesh);

	ChunkMesh(ChunkMesh&& other) noexcept;
	ChunkMesh& operator=(ChunkMesh&& other) noexcept;

	Mesh& getMeshForBlockID(unsigned char blockID);
private:
	ChunkMesh(const ChunkMesh& other) = delete;
	void operator=(const ChunkMesh& other) = delete;
};