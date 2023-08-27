#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <set>
#include <unordered_map>

#include "utils/atomics/AtomicVector.h"
#include "utils/atomics/AtomicSet.h"
#include "math/Vector.h"
#include "world/terrain/TerrainGenerator.h"

#define GENERATORS_COUNT 1

class World;
class Chunk;

class WorldGenerator
{
public:
	WorldGenerator(World* world);

	~WorldGenerator();

	void requestChunks(const std::vector<Vector> positions);
	void requestCleaning(Chunk* chunk);
	void requestCleaning(const std::vector<Chunk*> chunks);
	void requestCleaning(const std::set<Chunk*> chunks);

private:
	unsigned int seed;
	World* world;

	std::atomic_bool shouldStop;
	std::thread generatorThreads[GENERATORS_COUNT];
	std::thread cleaningThread;

	AtomicVector<Vector> requestedPositions;

	std::mutex cleaningMutex;
	std::unordered_map<Vector, bool> cleaningProgressMap;
	std::set<Chunk*> dirtyChunks;

	TerrainGenerator* terrainGenerator;

	void generateTerrain();

	WorldGenerator(const WorldGenerator& other) = delete;
	void operator = (const WorldGenerator& other) = delete;
};