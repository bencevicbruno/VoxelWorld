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

#include "Options.h"

class World;
class ChunkMesher;

class ChunkGenerator
{
public:
	ChunkGenerator() = default;
	ChunkGenerator(World* world, ChunkMesher* chunkMesher);

	~ChunkGenerator();

	void requestChunk(const Vector& position);
	void requestChunks(const std::vector<Vector>& positions);

private:
	unsigned int seed;
	World* world;
	ChunkMesher* chunkMesher;

	bool shouldStop;
	std::thread generatorThreads[GENERATORS_COUNT];
	TerrainGenerator* terrainGenerator;

	std::mutex requestedPositionsMutex;
	std::vector<Vector> requestedPositions;

	void runLoop();
	void generateTerrain(const Vector& chunkPosition);

	ChunkGenerator(const ChunkGenerator& other) = delete;
	void operator = (const ChunkGenerator& other) = delete;
};