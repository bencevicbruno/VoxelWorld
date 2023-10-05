#pragma once

#include <mutex>
#include <optional>

#include "renderer/camera/Camera.h"
#include "math/Vector.h"
#include "math/Matrix.h"
#include "renderer/models/Mesh.h"
#include "world/chunk/ChunkMesh.h"
#include "renderer/shader/ShaderProgram.h"
#include "world/utils/BlockRegistry.h"

#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128

class World;

class Chunk
{
public:
	Chunk(const Vector& position, World* world, unsigned char* blocks);
	Chunk(Chunk&& other) noexcept;

	Chunk& operator=(Chunk&& other) noexcept;

	~Chunk();

	void generateBuffers();
	bool isVisible(const Camera& camera) const;
	void render() const;
	void renderTransparent() const;
	void renderWater() const;

	Vector getPosition() const;
	bool containsPosition(int x, int y, int z) const;
	std::optional<unsigned char> getBlockAt(int x, int y, int z) const;
	unsigned char getBlockAt(const Vector& position) const;
	void setBlock(const Vector& position, const Block& block);
	void setBlock(const Vector& position, unsigned char block);

	bool isDirty() const;
	void markDirty(bool isDirty);

	std::vector<Vector> getNeighbouringPositions() const;

	void updateChunkMesh(ChunkMesh* chunkMesh);
	static int coordsToOffset(int x, int y, int z);
	static int coordsToOffset(float x, float y, float z);
	static int coordsToOffset(const Vector& coords);
private:
	Vector position;
	World* world;

	unsigned char* blocks;

	Mesh mesh;
	Mesh transparentMesh;
	Mesh waterMesh;

	bool dirty;

	Chunk(const Chunk& other) = delete;
	void operator=(const Chunk& other) = delete;

	friend class BasicChunkMeshBuilder;
	friend class PerChunkOptimizedChunkMeshBuilder;
	friend class OptimalChunkMeshBuilder;
};