#include "world/chunk/Chunk.h"

#include "Options.h"
#include "devices/Window.h"
#include "math/PerlinNoiseGenerator.h"
#include "renderer/models/BlockMesh.h"
#include "world/Blocks.h"
#include "world/World.h"
#include "world/mesh/BasicChunkMeshBuilder.h"
#include "world/mesh/PerChunkOptimizedChunkMeshBuilder.h"
#include "world/mesh/OptimalChunkMeshBuilder.h"

#include "world/utils/BlockRegistry.h"

Chunk::Chunk(const Vector& position, World* world, unsigned char* blocks) :
	position(position),
	world(world),
	blocks(blocks),
	dirty(false)
{}

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

void Chunk::generateBuffers()
{
	mesh.generateBuffers();
	transparentMesh.generateBuffers();
	waterMesh.generateBuffers();
}

bool Chunk::isVisible(const Camera& camera) const
{
	//return true;
	Matrix chunkTranslationMatrix = Matrix::GetTranslation(position * 16);
	Matrix chunkMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * chunkTranslationMatrix;

	int horizontalCoordinates[] = { 0, CHUNK_WIDTH };
	constexpr float threshold = 1.1;

	for (int x = 0; x < 2; x++)
	{
		for (int z = 0; z < 2; z++)
		{
			for (int y = 0; y <= CHUNK_HEIGHT; y += CHUNK_WIDTH)
			{
				Vector transformedCorner = chunkMatrix * Vector(x, y, z);
				Vector normalizedCorner = transformedCorner / transformedCorner.w;

				if (normalizedCorner.x >= -threshold && normalizedCorner.x <= threshold &&
					normalizedCorner.y >= -threshold && normalizedCorner.y <= threshold &&
					normalizedCorner.z >= -threshold && normalizedCorner.z <= threshold) {
					return true;
				}
			}
		}
	}

	return false;
}

void Chunk::updateChunkMesh(ChunkMesh* chunkMesh)
{
	mesh = std::move(chunkMesh->solidMesh);
	mesh.generateBuffers();

	transparentMesh = std::move(chunkMesh->transparentMesh);
	transparentMesh.generateBuffers();

	waterMesh = std::move(chunkMesh->waterMesh);
	waterMesh.generateBuffers();

	delete chunkMesh;
}

int Chunk::coordsToOffset(int x, int y, int z)
{
	if (x < 0 || x >= CHUNK_WIDTH || z < 0 || z >= CHUNK_WIDTH || y < 0 || y >= CHUNK_HEIGHT)
	{
		throw "Illegal coordinates";
	}

	return y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z;
}

int Chunk::coordsToOffset(float x, float y, float z)
{
	return coordsToOffset(int(x), int(y), int(z));
}

int Chunk::coordsToOffset(const Vector& coords)
{
	return coordsToOffset(coords.x, coords.y, coords.z);
}

void Chunk::render() const
{
	mesh.render();
}

void Chunk::renderTransparent() const
{
	transparentMesh.render();
}

void Chunk::renderWater() const
{
	waterMesh.render();
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
	if (x < 0 || y < 0 || z < 0 || x >= CHUNK_WIDTH || y >= CHUNK_HEIGHT || z >= CHUNK_WIDTH)
	{
		return BLOCK_AIR;// world->getBlockAt(position.x + x, position.y + y, position.z + z);
	}

	return blocks[y * CHUNK_WIDTH * CHUNK_WIDTH + x * CHUNK_WIDTH + z];
}

unsigned char Chunk::getBlockAt(const Vector& position) const
{
	return blocks[int(position.y * CHUNK_WIDTH * CHUNK_WIDTH + position.x * CHUNK_WIDTH + position.z)];
}

void Chunk::setBlock(const Vector& position, const Block& block)
{
	setBlock(position, block.id);
}

void Chunk::setBlock(const Vector& position, unsigned char block)
{
	blocks[coordsToOffset(position)] = block;
	dirty = true;
}

bool Chunk::isDirty() const
{
	return dirty;
}

void Chunk::markDirty(bool isDirty)
{
	this->dirty = isDirty;
}

std::vector<Vector> Chunk::getNeighbouringPositions() const
{
	return {
		position.goNorth(),
		position.goSouth(),
		position.goEast(),
		position.goWest()
	};
}