#pragma once

#include <optional>

#include "math/Vector.h"
#include "renderer/models/Mesh.h"
#include "renderer/shader/ShaderProgram.h"
#include "world/utils/BlockRegistry.h"

#define CHUNK_SIZE 16

class World;

class Chunk
{
public:
	Chunk(const Vector& position, World* world, unsigned char* blocks);
	Chunk(Chunk&& other) noexcept;

	Chunk& operator=(Chunk&& other) noexcept;

	~Chunk();

	void generateBuffers();
	void render() const;
	void renderTransparent() const;

	Vector getPosition() const;
	bool containsPosition(int x, int y, int z) const;
	std::optional<unsigned char> getBlockAt(int x, int y, int z) const;
private:
	Vector position;
	World* world;

	unsigned char* blocks;

	Mesh mesh;
	Mesh transparentMesh;

	bool isDirty;

	bool shouldRenderSide(const Block& block, const Block& neighbouringBlock) const;

	void generateMesh();

	Chunk(const Chunk& other) = delete;
	void operator=(const Chunk& other) = delete;
};