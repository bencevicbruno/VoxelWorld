#include "world/WorldGenerator.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

#include "math/PerlinNoiseGenerator.h"
#include "world/Blocks.h"
#include "world/Chunk.h"
#include "world/World.h"
#include "world/terrain/FirstWorkingTerrainGenerator.h"
#include "world/terrain/BiomeNoiseTerrainGenerator.h"
#include "world/terrain/MinecraftTerrainGenerator.h"

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
	//generatorThread.detach();
}

WorldGenerator::~WorldGenerator()
{
	shouldStop = true;

	for (int i = 0; i < GENERATORS_COUNT; i++)
	{
		if (!generatorThreads[i].joinable()) continue;
		generatorThreads[i].join();
	}

	delete terrainGenerator;
}

void WorldGenerator::requestChunks(const std::vector<Vector> positions, const Vector& currentPosition)
{
	if (positions.size() == 0) return;

	requestedPositions.perform([&positions, &currentPosition](std::vector<Vector>& requestedPositions) -> void
		{
			for (const Vector& position : positions)
			{
				requestedPositions.push_back(position);
			}

			std::sort(requestedPositions.begin(), requestedPositions.end(), [&currentPosition](const Vector& first, const Vector& second) -> bool
				{
					float firstDistance = first.distanceTo2(currentPosition);
					float secondDistance = second.distanceTo2(currentPosition);

					return firstDistance > secondDistance;
				});

		});
}

void WorldGenerator::generateTerrain()
{
	std::optional<Vector> positionToGenerate;

	while (!shouldStop)
	{
		if (positionToGenerate = requestedPositions.popLast())
		{
			world->addChunk(new Chunk(*positionToGenerate, world, terrainGenerator->generateTerrain(seed, *positionToGenerate)));
		}
	}
}