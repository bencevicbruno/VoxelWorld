#pragma once

#include <map>
#include <string>

#include "world/Blocks.h"
#include "renderer/models/BlockMesh.h"

#define REGISTRY_BLOCK_LIMIT 256

enum class BlockOpacity
{
	OPAQUE, SEE_THROUGH, TRANSPARENT, EMPTY
};

struct Block
{
	unsigned char id;
	BlockMesh mesh;
	BlockOpacity opacity;

	Block() :
		id(0),
		mesh(UVPosition::FromAtlas(15, 15)),
		opacity(BlockOpacity::EMPTY)
	{}
	Block(unsigned char id, BlockMesh&& mesh, BlockOpacity opacity) :
		id(id),
		mesh(std::move(mesh)),
		opacity(opacity)
	{}

	bool operator==(const Block& other) const
	{
		return this->id == other.id;
	}

	bool operator!=(const Block& other) const
	{
		return this->id != other.id;
	}
};

class BlockRegistry
{
public:
	static BlockRegistry& GetInstance();
	BlockRegistry();

	void registerBlock(unsigned char id, BlockMesh&& mesh, BlockOpacity opacity);
	Block& getBlockByID(unsigned char id);

private:
	Block registry[REGISTRY_BLOCK_LIMIT];

	BlockRegistry(const BlockRegistry& other) = delete;
	void operator=(const BlockRegistry& other) = delete;
};