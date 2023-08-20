#include "world/Chunk.h"

#include "math/PerlinNoiseGenerator.h"
#include "renderer/models/BlockMesh.h"
#include "world/Blocks.h"
#include "world/World.h"

#include "world/utils/BlockRegistry.h"

Chunk::Chunk(const Vector& position, World* world, unsigned char* blocks) :
	position({ position.x, position.y, position.z, 0 }),
	world(world),
	blocks(blocks),
	isDirty(false)
{
	generateMesh();
}

Chunk::Chunk(Chunk&& other) noexcept
{
	*this = std::move(other);
}

Chunk& Chunk::operator=(Chunk&& other) noexcept
{
	if (this == &other) return *this;

	position = std::move(other.position);
	mesh = std::move(mesh);
	blocks = std::exchange(other.blocks, nullptr);

	return *this;
}

Chunk::~Chunk()
{
	delete[] blocks;
}

void Chunk::generateMesh()
{
	BlockRegistry& blockRegistry = BlockRegistry::GetInstance();

	for (int x = 0; x < CHUNK_SIZE; x++)
	{
		for (int y = 0; y < CHUNK_SIZE; y++)
		{
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				unsigned char currentBlockID = blocks[x * 16 * 16 + z * 16 + y];
				if (currentBlockID == 0) continue;

				Block& currentBlock = blockRegistry.getBlockByID(currentBlockID);
				Mesh& mesh = currentBlock.opacity == BlockOpacity::OPAQUE ? this->mesh : transparentMesh;

				// BOTTOM FACE
				if (auto blockID = getBlockAt(x, y - 1, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getBottomFace({ x, y, z }));
					}
				}
				
				// TOP FACE
				if (auto blockID = getBlockAt(x, y + 1, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getTopFace({ x, y, z }));
					}
				}

				// NORTH FACE
				if (auto blockID = getBlockAt(x, y, z - 1))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getNorthFace({ x, y, z }));
					}
				}

				// SOUTH FACE
				if (auto blockID = getBlockAt(x, y, z + 1))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getSouthFace({ x, y, z }));
					}
				}

				// WEST FACE
				if (auto blockID = getBlockAt(x - 1, y, z))
				{
					if (shouldRenderSide(currentBlock, blockRegistry.getBlockByID(*blockID)))
					{
						mesh.addMesh(currentBlock.mesh.getWestFace({ x, y, z }));
					}
				}

				// EAST FACE
				if (auto blockID = getBlockAt(x + 1, y, z))
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

void Chunk::generateBuffers()
{
	mesh.generateBuffers();
	transparentMesh.generateBuffers();
}

void Chunk::render() const
{
	mesh.render();
}

void Chunk::renderTransparent() const
{
	transparentMesh.render();
}

bool Chunk::shouldRenderSide(const Block& block, const Block& neighbouringBlock) const
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

Vector Chunk::getPosition() const
{
	return position;
}

bool Chunk::containsPosition(int x, int y, int z) const
{
	return x >= this->position.x
		&& x < (this->position.x + 16)
		&& y >= this->position.y
		&& y < (this->position.y + 16)
		&& z >= this->position.z
		&& z < (this->position.z + 16);
}

std::optional<unsigned char> Chunk::getBlockAt(int x, int y, int z) const
{
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE)
	{
		return BLOCK_AIR;// world->getBlockAt(position.x + x, position.y + y, position.z + z);
	}

	return blocks[x * 16 * 16 + z * 16 + y];
}