#pragma once

#include <mutex>
#include <tuple>
#include <vector>

#include "renderer/wrappers/Texture.h"
#include "utils/atomics/AtomicVector.h"
#include "utils/atomics/AtomicMap.h"
#include "utils/atomics/AtomicSet.h"
#include "world/chunk/Chunk.h"
#include "world/chunk/ChunkMesh.h"
#include "world/chunk/ChunkMesher.h"
#include "world/chunk/ChunkGenerator.h"

class World
{
public:
	World(unsigned int seed, int renderRadius);

	~World();

	unsigned int getSeed() const;

	void addChunk(Chunk* chunk, ChunkMesh* chunkMesh);
	void addChunks(const std::vector<std::tuple<Chunk*, ChunkMesh*>>& chunks);
	void addCleanChunk(Chunk* chunk, ChunkMesh* chunkMesh);
	void addCleanChunks(const std::vector<std::tuple<Chunk*, ChunkMesh*>>& chunks);

	void update(const Vector& cameraPosition);
	void addPendingChunks();
	void updateCleanedChunkMeshes();
	void handlePendingBlocks();
	void handleDirtyChunks();

	void render();

	std::optional<unsigned char> getBlockAt(int x, int y, int z);
	void setBlock(const Vector& position, const Block& block);
	void setBlocks(const std::unordered_map<Vector, unsigned char>& blocks);
	void setBlocks(const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks);
	Chunk* getChunkAt(int x, int y, int z);
	Chunk* getChunkAt(const Vector& position);

	std::unordered_map<Vector, unsigned char> exchangePendingBlocks(const Vector& chunkPosition, const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& otherBlocks);
private:
	unsigned int seed;
	float time;
	int renderRadius;
	Texture textureAtlas;

	std::mutex pendingBlocksMutex;

	std::unordered_map<Vector, Chunk*> chunks;
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> pendingBlocks;

	ChunkMesher chunkMesher;
	ChunkGenerator chunkGenerator;

	AtomicVector<std::tuple<Chunk*, ChunkMesh*>> pendingChunks;
	AtomicVector<std::tuple<Chunk*, ChunkMesh*>> cleanedChunks;

	std::vector<Vector> getChunkPositionsToLoad() const;
};