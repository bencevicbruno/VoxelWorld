#pragma once

#include <mutex>
#include <vector>

#include "renderer/wrappers/Texture.h"
#include "utils/atomics/AtomicVector.h"
#include "utils/atomics/AtomicMap.h"
#include "utils/atomics/AtomicSet.h"
#include "world/Chunk.h"
#include "world/ChunkMesh.h"
#include "world/WorldGenerator.h"

class World
{
public:
	World();
	World(unsigned int seed, int renderRadius = 16);

	~World();

	unsigned int getSeed() const;

	void addChunk(Chunk* chunk);
	void addChunks(const std::vector<Chunk*>& chunks);
	void cleanChunk(Chunk* chunk, ChunkMesh&& newChunkMesh);

	void update(const Vector& cameraPosition);
	void addPendingChunks();
	void handleDirtyChunks();
	void render();

	std::optional<unsigned char> getBlockAt(int x, int y, int z);
	void setBlock(const Vector& position, const Block& block);
	void setBlocks(const std::unordered_map<Vector, unsigned char>& blocks);
	void setBlocks(const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks);
	Chunk* getChunkAt(int x, int y, int z);
	Chunk* getChunkAt(const Vector& position);
private:
	unsigned int seed;
	float time;
	int renderRadius;
	Texture textureAtlas;

	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> pendingBlocks;
	std::unordered_map<Vector, Chunk*> chunks;
	WorldGenerator worldGenerator;

	AtomicVector<Chunk*> pendingChunks;
	AtomicMap<Chunk*, ChunkMesh> cleanChunks;

	std::vector<Vector> getChunkPositionsToLoad() const;

	friend WorldGenerator;
};