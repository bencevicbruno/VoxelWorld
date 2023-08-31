#pragma once

#include <vector>

#include "world/chunk/Chunk.h"
#include "world/mesh/ChunkMeshBuilder.h"

#include "Options.h"

class World;

class ChunkMesher
{
public:
	ChunkMesher() = default;
	ChunkMesher(World* world);

	~ChunkMesher();

	void requestMesh(Chunk* chunk);
	void requestMeshes(const std::vector<Chunk*>& chunks);
	void requestRemesh(Chunk* chunk);
	void requestRemeshes(const std::vector<Chunk*>& chunks);

private:
	World* world;
	ChunkMeshBuilder* chunkMeshBuilder;

	std::atomic_bool shouldStop;
	std::thread mesherThread[MESHERS_COUNT];
	std::thread remesherThread[REMESHERS_COUNT];

	std::mutex meshesMutex;
	std::vector<Chunk*> requestedMeshes;

	std::mutex remeshesMutex;
	std::vector<Chunk*> requestedRemeshes;

	void runLoop(bool isForRemeshing);

	void generateMesh(Chunk* chunk);
	void regenerateMesh(Chunk* chunk);
};