#pragma once

#include <mutex>
#include <vector>

#include "renderer/wrappers/Texture.h"
#include "utils/atomics/AtomicVector.h"
#include "world/Chunk.h"
#include "world/WorldGenerator.h"

class World
{
public:
	World();
	World(unsigned int seed, const Vector& initialCameraPosition);

	~World();

	unsigned int getSeed() const;

	void addChunk(Chunk* chunk);
	void update(const Vector& cameraPosition);
	void render();

	std::optional<unsigned char> getBlockAt(int x, int y, int z);
	Chunk* getChunkAt(int x, int y, int z);

private:
	unsigned int seed;
	int prevCameraX, prevCameraY, prevCameraZ;
	int renderRadius;
	Texture textureAtlas;

	std::unordered_map<Vector, Chunk*> chunks;
	AtomicVector<Chunk*> chunksToAdd;
	std::vector<Vector> loadedPositions;
	WorldGenerator worldGenerator;

	std::vector<Vector> getChunkPositionsForPosition(const Vector& position);
	//std::vector<Vector> getChunkPositionsToUnload(const std::vector<Vector> oldPositions, const std::vector<Vector> newPositions);
	std::vector<Vector> getChunkPositionToLoad(const std::vector<Vector> newPositions);
};