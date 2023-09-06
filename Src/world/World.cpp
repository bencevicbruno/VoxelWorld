#include "world/World.h"

#include "Options.h"

#include <stdio.h>

#include "utils/debug/Debug.h"
#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"
#include "utils/image/ImageManager.h"
#include "world/chunk/ChunkMesh.h"
#include "world/chunk/Chunk.h"
#include "renderer/models/BlockMesh.h"

World::World(unsigned int seed, int renderRadius) :
	seed(seed),
	renderRadius(renderRadius),
	textureAtlas(Texture::CreateFromImage("atlas.png")),
	chunkMesher(ChunkMesher(this)),
	chunkGenerator(ChunkGenerator(this, &chunkMesher)),
	time(0)
{
	textureAtlas.setWrapAndFilterForGUI();
	chunkGenerator.requestChunks(getChunkPositionsToLoad());
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

void World::addChunk(Chunk* chunk, ChunkMesh* chunkMesh)
{
	addChunks({ {chunk, std::move(chunkMesh)} });
}

void World::addChunks(const std::vector<std::tuple<Chunk*, ChunkMesh*>>& chunks)
{
	pendingChunks.append(chunks);
}

void World::addCleanChunk(Chunk* chunk, ChunkMesh* chunkMesh)
{
	addCleanChunks({ {chunk, std::move(chunkMesh)} });
}

void World::addCleanChunks(const std::vector<std::tuple<Chunk*, ChunkMesh*>>& chunks)
{
	cleanedChunks.append(chunks);
}

void World::update(const Vector& cameraPosition)
{
	updateCleanedChunkMeshes();
	handlePendingBlocks();
	addPendingChunks();
	handleDirtyChunks();

	time += 0.01;
}

void World::addPendingChunks()
{
	pendingChunks.perform([this](std::vector<std::tuple<Chunk*, ChunkMesh*>>& pendingChunks) -> void
		{
			for (auto& [chunk, chunkMesh] : pendingChunks)
			{
				chunk->updateChunkMesh(chunkMesh);
				this->chunks[chunk->getPosition()] = chunk;
			}

			for (auto& [chunk, chunkMesh] : pendingChunks)
			{
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

void World::updateCleanedChunkMeshes()
{
	cleanedChunks.perform([this](std::vector<std::tuple<Chunk*, ChunkMesh*>>& cleanedChunks) -> void
		{
			for (auto& [chunk, chunkMesh] : cleanedChunks)
			{
				chunk->updateChunkMesh(chunkMesh);
				chunk->markDirty(false);
			}

			cleanedChunks.clear();
		});
}

void World::handlePendingBlocks()
{
	pendingBlocksMutex.lock();

	std::vector<Vector> checkedPositions;

	for (auto& [chunkPosition, blocks] : pendingBlocks)
	{
		if (Chunk* chunk = getChunkAt(chunkPosition))
		{
			for (auto& [blockPosition, blockID] : blocks)
			{
				chunk->setBlock(blockPosition, blockID);
			}

			checkedPositions.push_back(chunkPosition);
			blocks.clear();
			chunk->markDirty(true);
		}
	}

	for (const Vector& checkedPosition : checkedPositions)
	{
		pendingBlocks.erase(checkedPosition);
	}

	pendingBlocksMutex.unlock();
}

void World::handleDirtyChunks()
{
	std::vector<Chunk*> dirtyChunks;

	for (const auto& [position, chunk] : chunks)
	{
		if (chunk->isDirty())
		{
			dirtyChunks.push_back(chunk);
			chunk->markDirty(false);
		}
	}

	chunkMesher.requestRemeshes(dirtyChunks);
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
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first * 16));
		it.second->render();
	}

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	for (auto& it : chunks)
	{
		//if (!it.second->isVisible(camera)) continue;
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first * 16));
		it.second->renderTransparent();
	}
	glDisable(GL_ALPHA_TEST);

	glDisable(GL_CULL_FACE);
	shaderProgram.setFloat("waveAmplifier", 1.5 / 16.0);
	shaderProgram.setBool("enablePhong", true);
	for (auto& it : chunks)
	{
		//if (!it.second->isVisible(camera)) continue;
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first * 16));
		it.second->renderWater();
	}
}

std::optional<unsigned char> World::getBlockAt(int x, int y, int z)
{
	Vector chunkPosition = Vector::GlobalToChunk({ x, y, z });
	Vector localPosition = Vector::GlobalToChunk({ x, y, z });

	Chunk* chunk = getChunkAt(chunkPosition);

	if (chunk == nullptr) return {};

	return chunk->getBlockAt(localPosition);
}

void World::setBlock(const Vector& position, const Block& block)
{
	pendingBlocksMutex.lock();
	Vector chunkPosition = Vector::GlobalToChunk(position);
	Vector localPosition = position % 16;

	Chunk* chunk = getChunkAt(chunkPosition);
	if (chunk == nullptr)
	{
		pendingBlocks[chunkPosition][localPosition] = block.id;
		pendingBlocksMutex.unlock();
		return;
	}

	chunk->setBlock(localPosition, block);
	pendingBlocksMutex.unlock();
}

void World::setBlocks(const std::unordered_map<Vector, unsigned char>& blocks)
{
	pendingBlocksMutex.lock();
	std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>> blocksPerChunk;

	for (auto& pair : blocks)
	{
		Vector chunkPosition = Vector::GlobalToChunk(pair.first);
		Vector localPosition = pair.first % 16;

		blocksPerChunk[chunkPosition][localPosition] = pair.second;
	}

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
		}
	}

	pendingBlocksMutex.unlock();
}

void World::setBlocks(const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& blocks)
{
	pendingBlocksMutex.lock();

	for (auto& [chunkPosition, chunkBlocks] : blocks)
	{
		if (Chunk* chunk = getChunkAt(chunkPosition))
		{
			for (auto& [blockPosition, blockID] : chunkBlocks)
			{
				chunk->setBlock(blockPosition, blockID);
			}
		}
		else
		{
			for (auto& [blockPosition, blockID] : chunkBlocks)
			{
				pendingBlocks[chunkPosition][blockPosition] = blockID;
			}
		}
	}
	pendingBlocksMutex.unlock();
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

std::unordered_map<Vector, unsigned char> World::exchangePendingBlocks(const Vector& chunkPosition, const std::unordered_map<Vector, std::unordered_map<Vector, unsigned char>>& otherBlocks)
{
	pendingBlocksMutex.lock();

	std::unordered_map<Vector, unsigned char> pendingBlocks = this->pendingBlocks[chunkPosition];
	this->pendingBlocks.erase(chunkPosition);

	for (auto& [otherChunkPosition, otherChunkBlocks] : otherBlocks)
	{
		this->pendingBlocks[otherChunkPosition] = otherChunkBlocks;
	}

	pendingBlocksMutex.unlock();

	return pendingBlocks;
}

std::vector<Vector> World::getChunkPositionsToLoad() const
{
	std::vector<Vector> positions;

	bool forTesting = false;

	if (forTesting)
	{
		int testRadius = 8;
		for (int x = -testRadius; x <= testRadius; x++)
		{
			for (int z = -testRadius; z <= testRadius; z++)
			{
				positions.push_back({ x, 0, z });
			}
		}
	}
	else
	{
		for (int x = -renderRadius; x <= renderRadius; x++)
		{
			for (int z = -renderRadius; z <= renderRadius; z++)
			{
				positions.push_back({ x, 0, z });
			}
		}
	}

	std::sort(positions.begin(), positions.end(), [](const Vector& lhs, const Vector& rhs) -> bool
		{
			return lhs.length2() > rhs.length2();
		});

	return positions;
}