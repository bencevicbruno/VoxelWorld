#include "world/chunk/ChunkMesh.h"

#include "world/Blocks.h"

ChunkMesh::ChunkMesh(Mesh&& solidMesh, Mesh&& transparentMesh, Mesh&& waterMesh) :
	solidMesh(std::move(solidMesh)),
	transparentMesh(std::move(transparentMesh)),
	waterMesh(std::move(waterMesh))
{}

ChunkMesh::ChunkMesh(ChunkMesh&& other) noexcept
{
	*this = std::move(other);
}

ChunkMesh& ChunkMesh::operator=(ChunkMesh&& other) noexcept
{
	if (this == &other) return *this;

	solidMesh = std::move(other.solidMesh);
	transparentMesh = std::move(other.transparentMesh);
	waterMesh = std::move(other.waterMesh);

	return *this;
}

Mesh& ChunkMesh::getMeshForBlockID(unsigned char blockID)
{
	switch (blockID)
	{
	case BLOCK_WATER:
		return waterMesh;
	case BLOCK_WATER_SURFACE:
		return waterMesh;
	case BLOCK_OAK_LEAVES:
		return transparentMesh;
	default:
		return solidMesh;
	}
}