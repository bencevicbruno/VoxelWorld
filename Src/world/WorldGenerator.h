#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "utils/atomics/AtomicVector.h"
#include "math/Vector.h"
#include "world/terrain/TerrainGenerator.h"

#define GENERATORS_COUNT 6

class World;
class Chunk;

class WorldGenerator
{
public:
	WorldGenerator(World* world);

	~WorldGenerator();

	void requestChunks(const std::vector<Vector> positions, const Vector& currentPosition);

private:
	unsigned int seed;
	World* world;

	std::atomic_bool shouldStop;
	std::thread generatorThreads[GENERATORS_COUNT];

	AtomicVector<Vector> requestedPositions;

	TerrainGenerator* terrainGenerator;

	void generateTerrain();
	Chunk* generateChunk(const Vector& position) const;

	WorldGenerator(const WorldGenerator& other) = delete;
	void operator = (const WorldGenerator& other) = delete;
};