#include "world/World.h"

#include "Options.h"

#include <stdio.h>

#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"
#include "utils/image/ImageManager.h"
#include "world/ChunkMesh.h"
#include "world/Chunk.h"
#include "renderer/models/BlockMesh.h"

World::World() :
	renderRadius(8),
	textureAtlas(Texture::CreateFromImage("atlas.png")),
	worldGenerator({ this }),
	time(0)
{
	std::srand((unsigned int)std::time(NULL));
	this->seed = (unsigned int)rand();
}

World::World(unsigned int seed, int renderRadius) :
	seed(seed),
	renderRadius(renderRadius),
	textureAtlas(Texture::CreateFromImage("atlas.png")),
	worldGenerator({this}),
	time(0)
{
	textureAtlas.setWrapAndFilterForGUI();
	worldGenerator.requestChunks(getChunkPositionsToLoad());
}

World::~World()
{
	for (auto& it : chunks) {
		
		delete it.second;
	}
}

unsigned int World::getSeed() const
{
	return seed;
}

void World::addChunk(Chunk* chunk)
{
	pendingChunks.append(chunk);
}

void World::addChunks(const std::vector<Chunk*>& chunks)
{
	pendingChunks.append(chunks);
}

void World::cleanChunk(Chunk* chunk, ChunkMesh&& chunkMesh)
{
	cleanChunks.set(chunk, std::move(chunkMesh));
}

void World::update(const Vector& cameraPosition)
{
	addPendingChunks();

	cleanChunks.perform([this](auto& cleanChunks) -> void
		{
			for (auto& [chunk, newMesh] : cleanChunks)
			{
				chunk->updateChunkMesh(std::move(newMesh), true);
			}

			cleanChunks.clear();
		});
	
	for (auto& [position, blocks] : pendingBlocks)
	{
		if (blocks.empty()) continue;

		if (Chunk* chunk = getChunkAt(position))
		{
			for (auto& [blockPosition, block] : blocks)
			{
				chunk->setBlock(blockPosition, block);
			}

			chunk->markDirty(true);
		}

		blocks.clear();
	}

	handleDirtyChunks();
	time += 0.01;
}

void World::addPendingChunks()
{
	pendingChunks.perform([this](std::vector<Chunk*>& pendingChunks) -> void
		{
			for (Chunk* chunk : pendingChunks)
			{
				chunk->generateBuffers();
				this->chunks[chunk->getPosition()] = chunk;

				for (const Vector& position : chunk->getNeighbouringPositions())
				{
					if (Chunk* neighbourChunk = this->getChunkAt(position))
					{
						neighbourChunk->markDirty(true);
					}
				}
			}

			pendingChunks.clear();
		});
}

void World::handleDirtyChunks()
{
	std::set<Chunk*> dirtyChunks;

	for (const auto& [position, chunk] : chunks)
	{
		if (chunk->isDirty())
		{
			dirtyChunks.insert(chunk);
		}
	}

	worldGenerator.requestCleaning(dirtyChunks);
}

void World::render()
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	shaderProgram.use();
	Matrix projectionViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
	shaderProgram.setMatrix("projection_view", projectionViewMatrix);
	shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setFloat("time", time);

	textureAtlas.bindToUnit(0);

	if (ENABLE_FACE_CULLING)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	shaderProgram.setFloat("waveAmplifier", 0);
	shaderProgram.setBool("enablePhong", false);
	for (auto& it : chunks)
	{
		//if (!it.second->isVisible(camera)) continue;
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first));
		it.second->render();
	}

	for (auto& it : chunks)
	{
		//if (!it.second->isVisible(camera)) continue;
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first));
		it.second->renderTransparent();
	}

	shaderProgram.setFloat("waveAmplifier", 1.5 / 16.0);
	shaderProgram.setBool("enablePhong", true);
	glDisable(GL_CULL_FACE);
	for (auto& it : chunks)
	{
		//if (!it.second->isVisible(camera)) continue;
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first));
		it.second->renderWater();
	}
}

std::optional<unsigned char> World::getBlockAt(int x, int y, int z)
{
	int chunkX = (x < 0) ? ((x - 16) / 16) : (x / 16);
	int chunkY = (y < 0) ? ((y - 16) / 16) : (y / 16);
	int chunkZ = (z < 0) ? ((z - 16) / 16) : (z / 16);

	int localX = (x - chunkX + 16) % 16;
	int localY = (y - chunkY + 16) % 16;
	int localZ = (z - chunkZ + 16) % 16;

	Chunk* chunk = getChunkAt(chunkX, chunkY, chunkZ);
	//printf("%d %d %d; %d %d %d\n", chunkX, chunkY, chunkZ, localX, localY, localZ);
	if (chunk == nullptr) return {};
	return chunk->getBlockAt(localX, localY, localZ);
}

void World::setBlock(const Vector& position, const Block& block)
{
	Vector chunkPosition = Vector::GlobalToChunk(position);
	Vector localPosition = position % 16;

	Chunk* chunk = getChunkAt(chunkPosition);
	if (chunk == nullptr)
	{
		pendingBlocks[chunkPosition][localPosition] = block.id;
		return;
	}

	chunk->setBlock(localPosition, block);
	chunk->markDirty(true);
	worldGenerator.requestCleaning(chunk);
}

void World::setBlocks(const std::unordered_map<Vector, unsigned char>& blocks)
{
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> blocksPerChunk;

	for (auto& pair : blocks)
	{
		Vector chunkPosition = Vector::GlobalToChunk(pair.first);
		Vector localPosition = pair.first % 16;

		blocksPerChunk[chunkPosition][localPosition] = pair.second;
	}

	std::vector<Chunk*> dirtyChunks;
	for (const auto& [chunkPosition, blocks] : blocksPerChunk)
	{
		Chunk* chunk = getChunkAt(chunkPosition);
		if (chunk == nullptr)
		{
			for (const auto& [blockPosition, blockID] : blocks)
			{
				pendingBlocks[chunkPosition][blockPosition] = blockID;
			}
			continue;
		}

		for (const auto& [blockPosition, blockID] : blocks)
		{
			chunk->setBlock(blockPosition, blockID);
			chunk->markDirty(true);
			worldGenerator.requestCleaning(chunk);
		}

		dirtyChunks.push_back(chunk);
	}

	blocksPerChunk.clear();

	worldGenerator.requestCleaning(dirtyChunks);
}

void World::setBlocks(const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks)
{
	for (auto& [chunkPosition, chunkBlocks] : blocks)
	{
		if (Chunk* chunk = getChunkAt(chunkPosition))
		{
			for (auto& [blockPosition, blockID] : chunkBlocks)
			{
				chunk->setBlock(blockPosition, blockID);
			}

			chunk->markDirty(true);
		}
		else
		{
			pendingBlocks[chunkPosition] = chunkBlocks;
		}
	}
}

//std::optional<unsigned char> World::getBlockAt(int x, int y, int z)
//{
//	if (y < 0) return {};
//
//	if (std::optional<const Chunk*> targetChunk = chunks.firstWhere([x, y, z](const Chunk* chunk) -> bool { return chunk->containsPosition(x, y, z); }))
//	{
//		return (*targetChunk)->getBlockAt((x % 16 + 16) % 16, (y % 16 + 16) % 16, (z % 16 + 16) % 16);
//	}
//	for (Chunk* chunk : chunks)
//	{
//		if (chunk->containsPosition(x, y, z))
//		{
//			//printf("mapping %d %d %d to %d %d %d \n", x, y, z, (x % 16 + 16) % 16, (y % 16 + 16) % 16, (z % 16 + 16) % 16);
//			return chunk->getBlockAt((x % 16 + 16) % 16, (y % 16 + 16) % 16, (z % 16 + 16) % 16);
//		}
//	}
//
//	return {};
//}

Chunk* World::getChunkAt(const Vector& position)
{
	return getChunkAt(position.x, position.y, position.z);
}

Chunk* World::getChunkAt(int x, int y, int z)
{
	try
	{
		Chunk* result = chunks.at({ x, y, z });
		return result;
	}
	catch (const std::out_of_range&)
	{
		return nullptr;
	}
}

std::vector<Vector> World::getChunkPositionsToLoad() const
{
	std::vector<Vector> positions;

	bool forTesting = true;

	if (forTesting)
	{
		int testRadius = 4;
		for (int x = -testRadius; x <= testRadius; x++)
		{
			for (int z = -testRadius; z <= testRadius; z++)
			{
				positions.push_back({ x * 16, 0, z * 16 });
			}
		}
	}
	else
	{
		for (int x = -renderRadius; x <= renderRadius; x++)
		{
			for (int z = -renderRadius; z <= renderRadius; z++)
			{
				positions.push_back({ x * 16, 0, z * 16 });
			}
		}
	}

	std::sort(positions.begin(), positions.end(), [](const Vector& lhs, const Vector& rhs) -> bool
		{
			return lhs.length2() > rhs.length2();
		});

	return positions;
}