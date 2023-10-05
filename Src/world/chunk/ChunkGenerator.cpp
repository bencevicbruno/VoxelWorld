#include "world/chunk/ChunkGenerator.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <set>

#include "utils/debug/Debug.h"
#include "math/PerlinNoiseGenerator.h"
#include "world/Blocks.h"
#include "world/chunk/Chunk.h"
#include "world/World.h"
#include "world/terrain/FirstWorkingTerrainGenerator.h"
#include "world/terrain/BiomeNoiseTerrainGenerator.h"
#include "world/terrain/MinecraftTerrainGenerator.h"
#include "world/terrain/3DNoiseExampleTerrainGeneratorr.h"
#include "world/decorator/LakeDecorator.h"
#include "world/decorator/SurfaceBlockDecorator.h"
#include "world/decorator/LakeSurfaceDecorator.h"
#include "world/decorator/ForestDecorator.h"
#include "world/decorator/SurfaceVegetationDecorator.h"
#include "world/decorator/SpecialDecorator.h"

ChunkGenerator::ChunkGenerator(World* world, ChunkMesher* chunkMesher) :
	seed(world->getSeed()),
	world(world),
	chunkMesher(chunkMesher),
	shouldStop(false),
	//terrainGenerator(new ThreeDNoiseExampleTerrainGenerator())
	terrainGenerator(new FinalTerrainGenerator(world->getSeed()))
{
	if (ENABLE_DECORATIONS)
	{
		int lakeLevel = 20;

		terrainDecorators.push_back(new LakeDecorator(lakeLevel));
		terrainDecorators.push_back(new SurfaceBlockDecorator(seed, lakeLevel));

		if (ENABLE_VEGETATION)
		{
			terrainDecorators.push_back(new LakeSurfaceDecorator(lakeLevel));
			terrainDecorators.push_back(new ForestDecorator(seed, lakeLevel));
			terrainDecorators.push_back(new SurfaceVegetationDecorator(seed));
			terrainDecorators.push_back(new SpecialDecorator(lakeLevel));
		}
	}

	for (int i = 0; i < GENERATORS_COUNT; i++)
	{
		generatorThreads[i] = std::thread(&ChunkGenerator::runLoop, this);
	}
}

ChunkGenerator::~ChunkGenerator()
{
	shouldStop = true;

	for (int i = 0; i < GENERATORS_COUNT; i++)
	{
		if (!generatorThreads[i].joinable()) continue;
		generatorThreads[i].join();
	}

	for (TerrainDecorator* decorator : terrainDecorators)
	{
		delete decorator;
	}

	delete terrainGenerator;
}

void ChunkGenerator::requestChunk(const Vector& position)
{
	requestChunks({ position });
}

void ChunkGenerator::requestChunks(const std::vector<Vector>& positions)
{
	std::lock_guard<std::mutex> guard(requestedPositionsMutex);

	for (const Vector& position : positions)
	{
		requestedPositions.push_back(position);
	}

	std::sort(requestedPositions.begin(), requestedPositions.end(), [](const Vector& lhs, const Vector& rhs) -> bool
		{
			return lhs.length2() > rhs.length2();
		});
}

void ChunkGenerator::runLoop()
{
	while (!shouldStop)
	{
		requestedPositionsMutex.lock();

		if (requestedPositions.empty())
		{
			this->requestedPositionsMutex.unlock();

			using namespace std::chrono_literals;
			std::this_thread::sleep_for(100ms);

			continue;
		}

		Vector positionToGenerate = requestedPositions.back();
		requestedPositions.pop_back();
		this->requestedPositionsMutex.unlock();

		generateTerrain(positionToGenerate);
	}
}

void ChunkGenerator::generateTerrain(const Vector& chunkPosition)
{
	int* heightMap = terrainGenerator->generateHeightMap(int(chunkPosition.x), int(chunkPosition.z));

	// 1) Generate the terrain
	unsigned char* blocks = terrainGenerator->generateTerrain(seed, chunkPosition, heightMap);

	// 2) Add decorations to this chunk
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> neighbourDecorations;

	for (TerrainDecorator* decorator : terrainDecorators)
	{
		auto decoration = decorator->decorate(chunkPosition, blocks, heightMap);

		for (auto& [chunkPosition, blocks] : decoration)
		{
			neighbourDecorations[chunkPosition].insert(blocks.begin(), blocks.end());
		}
	}

	delete[] heightMap;

	// 3) Add any blocks that are waiting to be added to this chunk
	std::unordered_map<Vector, unsigned char> pendingBlocks = world->exchangePendingBlocks(chunkPosition, neighbourDecorations);
	for (auto& [position, blockID] : pendingBlocks)
	{
		blocks[Chunk::coordsToOffset(position)] = blockID;
	}

	Chunk* newChunk = new Chunk(chunkPosition, world, blocks);
	chunkMesher->requestMesh(newChunk);
}