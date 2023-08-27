#include "world/WorldGenerator.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>
#include <set>

#include "math/PerlinNoiseGenerator.h"
#include "world/Blocks.h"
#include "world/Chunk.h"
#include "world/World.h"
#include "world/terrain/FirstWorkingTerrainGenerator.h"
#include "world/terrain/BiomeNoiseTerrainGenerator.h"
#include "world/terrain/MinecraftTerrainGenerator.h"
#include "world/decoration/OakTreeWorldDecoration.h"

WorldGenerator::WorldGenerator(World* world) :
	seed(world->getSeed()),
	world(world),
	shouldStop(false),
	//terrainGenerator(new FirstWorkingTerrainGenerator(world->getSeed()))
	terrainGenerator(new MinecraftNoiseGenerator(world->getSeed()))
{
	for (int i = 0; i < GENERATORS_COUNT; i++)
	{
		generatorThreads[i] = std::thread(&WorldGenerator::generateTerrain, this);
	}
}

WorldGenerator::~WorldGenerator()
{
	shouldStop = true;

	for (int i = 0; i < GENERATORS_COUNT; i++)
	{
		if (!generatorThreads[i].joinable()) continue;
		generatorThreads[i].join();
	}

	if (cleaningThread.joinable())
	{
		cleaningThread.join();
	}

	delete terrainGenerator;
}

void WorldGenerator::requestChunks(const std::vector<Vector> positions)
{
	requestedPositions.append(positions);
}

void WorldGenerator::requestCleaning(Chunk* chunk)
{
	std::lock_guard<std::mutex> guard(cleaningMutex);

	dirtyChunks.insert(chunk);
}

void WorldGenerator::requestCleaning(const std::vector<Chunk*> chunks)
{
	std::lock_guard<std::mutex> guard(cleaningMutex);

	for (Chunk* chunk : chunks)
	{
		dirtyChunks.insert(chunk);
	}
}

void WorldGenerator::requestCleaning(const std::set<Chunk*> chunks)
{
	std::lock_guard<std::mutex> guard(cleaningMutex);

	for (Chunk* chunk : chunks)
	{
		dirtyChunks.insert(chunk);
	}
}

void WorldGenerator::generateTerrain()
{
	PerlinNoiseGenerator decorationsNoiseGenerator(world->getSeed() * 0.1, 1);

	std::optional<Vector> positionToGenerate;
	Chunk* dirtyChunk = nullptr;

	while (!shouldStop)
	{
		if (positionToGenerate = requestedPositions.popLast())
		{
			int x = int((*positionToGenerate).x);
			int z = int((*positionToGenerate).z);

			//std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> decorations;

			auto treeBlocks = OakTreeWorldDecoration().getBlocks({x +  8, terrainGenerator->getHeight(x + 8, z + 8) + 1, z + 8 });
			//decorations.insert(treeBlocks.begin(), treeBlocks.end());


			Chunk* newChunk = new Chunk(*positionToGenerate, world, terrainGenerator->generateTerrain(seed, *positionToGenerate, treeBlocks[*positionToGenerate]));
			world->addChunk(newChunk);

			//decorations.erase(*positionToGenerate);
			
		}

		cleaningMutex.lock();
		if (!dirtyChunks.empty())
		{
			dirtyChunk = *dirtyChunks.begin();
			dirtyChunks.erase(dirtyChunk);
		}
		cleaningMutex.unlock();

		if (dirtyChunk != nullptr)
		{
			ChunkMesh chunkMesh = dirtyChunk->generateMesh();
			dirtyChunk->markDirty(false);
			world->cleanChunk(dirtyChunk, std::move(chunkMesh));
			dirtyChunk = nullptr;
		}
	}
}