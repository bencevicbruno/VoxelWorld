#include "world/chunk/ChunkMesher.h"

#include "world/World.h"
#include "world/mesh/BasicChunkMeshBuilder.h"
#include "world/mesh/PerChunkOptimizedChunkMeshBuilder.h"
#include "world/mesh/OptimalChunkMeshBuilder.h"

ChunkMesher::ChunkMesher(World* world) :
	world(world)
{
	switch (CHUNK_MESH_BUILDING_STRATEGY)
	{
	case BASIC_CHUNK_MESH_BULDING_STRATEGY:
		chunkMeshBuilder = new BasicChunkMeshBuilder();
		break;
	case PER_CHUNK_OPTIMIZED_CHUNK_MESH_BUILDING_STRATEGY:
		chunkMeshBuilder = new PerChunkOptimizedChunkMeshBuilder();
		break;
	case OPTIMAL_CHUNK_MESH_BUILDING_STRATEGY:
		chunkMeshBuilder = new OptimalChunkMeshBuilder();
		break;
	default:
		exit(69);
	}

	for (int i = 0; i < MESHERS_COUNT; i++)
	{
		mesherThread[i] = std::thread(&ChunkMesher::runLoop, this, false);
	}

	for (int i = 0; i < REMESHERS_COUNT; i++)
	{
		remesherThread[i] = std::thread(&ChunkMesher::runLoop, this, true);
	}
}

ChunkMesher::~ChunkMesher()
{
	shouldStop = true;

	for (int i = 0; i < MESHERS_COUNT; i++)
	{
		if (mesherThread[i].joinable())
			mesherThread[i].join();
	}
	
	for (int i = 0; i < REMESHERS_COUNT; i++)
	{
		if (remesherThread[i].joinable())
			remesherThread[i].join();
	}
	

	delete chunkMeshBuilder;
}

void ChunkMesher::requestMesh(Chunk* chunk)
{
	requestMeshes({chunk});
}

void ChunkMesher::requestMeshes(const std::vector<Chunk*>& chunks)
{
	std::lock_guard<std::mutex> guard(meshesMutex);

	for (Chunk* chunk : chunks)
	{
		requestedMeshes.push_back(chunk);
	}

	std::sort(requestedMeshes.begin(), requestedMeshes.end(), [](const Chunk* lhs, const Chunk* rhs) -> bool
		{
			return lhs->getPosition().length2() > rhs->getPosition().length2();
		});
	requestedMeshes.erase(std::unique(requestedMeshes.begin(), requestedMeshes.end()), requestedMeshes.end());
}

void ChunkMesher::requestRemesh(Chunk* chunk)
{
	requestRemeshes({ chunk });
}

void ChunkMesher::requestRemeshes(const std::vector<Chunk*>& chunks)
{
	std::lock_guard<std::mutex> guard(remeshesMutex);

	for (Chunk* chunk : chunks)
	{
		requestedRemeshes.push_back(chunk);
	}

	std::sort(requestedRemeshes.begin(), requestedRemeshes.end(), [](const Chunk* lhs, const Chunk* rhs) -> bool
		{
			return lhs->getPosition().length2() > rhs->getPosition().length2();
		});
	requestedRemeshes.erase(std::unique(requestedRemeshes.begin(), requestedRemeshes.end()), requestedRemeshes.end());
}

void ChunkMesher::runLoop(bool isForRemeshing)
{
	if (isForRemeshing)
	{
		while (!shouldStop)
		{
			remeshesMutex.lock();

			if (requestedRemeshes.empty())
			{
				remeshesMutex.unlock();
				continue;
			}

			Chunk* chunk = requestedRemeshes.back();
			requestedRemeshes.pop_back();
			remeshesMutex.unlock();

			regenerateMesh(chunk);
		}
	}
	else
	{
		while (!shouldStop)
		{
			meshesMutex.lock();

			if (requestedMeshes.empty())
			{
				meshesMutex.unlock();
				continue;
			}

			Chunk* chunk = requestedMeshes.back();
			requestedMeshes.pop_back();
			meshesMutex.unlock();

			generateMesh(chunk);
		}
	}
}

void ChunkMesher::generateMesh(Chunk* chunk)
{
	ChunkMesh* chunkMesh = chunkMeshBuilder->generateMesh(chunk);
	world->addChunk(chunk, chunkMesh);
}

void ChunkMesher::regenerateMesh(Chunk* chunk)
{
	ChunkMesh* chunkMesh = chunkMeshBuilder->generateMesh(chunk);
	world->addCleanChunk(chunk, chunkMesh);
}