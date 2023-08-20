#include "world/World.h"

#include <stdio.h>

#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"
#include "utils/image/ImageManager.h"
#include "renderer/models/BlockMesh.h"

World::World() :
	World(0, {})
{}

World::World(unsigned int seed, const Vector& initialCameraPosition) :
	seed(seed),
	prevCameraX(initialCameraPosition.x), prevCameraY(initialCameraPosition.y), prevCameraZ(initialCameraPosition.z),
	renderRadius(4),
	textureAtlas(Texture::CreateFromImage("atlas.png")),
	worldGenerator({this})
{
	textureAtlas.setWrapAndFilterForGUI();
	loadedPositions = getChunkPositionsForPosition({ float(prevCameraX), float(prevCameraY), float(prevCameraZ)});
	worldGenerator.requestChunks(loadedPositions, {prevCameraX, prevCameraY, prevCameraZ});
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
	chunksToAdd.append(chunk);
}

void World::update(const Vector& cameraPosition)
{
	int cameraX = int(cameraPosition.x) / 16;
	int cameraY = int(cameraPosition.y) / 16;
	int cameraZ = int(cameraPosition.z) / 16;

	chunksToAdd.perform([this](std::vector<Chunk*>& chunks) -> void
		{
			for (Chunk* chunk : chunks)
			{
				chunk->generateBuffers();
				this->chunks[chunk->getPosition()] = chunk;
			}

			chunks.clear();
		});

	if (cameraX != prevCameraX || cameraZ != prevCameraZ)// || cameraY != prevCameraY)
	{

		std::vector<Vector> newPositions = getChunkPositionsForPosition(cameraPosition);
		std::vector<Vector> positionsToLoad = std::move(getChunkPositionToLoad(newPositions));

		//worldGenerator.requestChunks(positionsToLoad, { cameraX, cameraY, cameraZ });

		/*chunks.removeAndDeleteWhere([&cameraPosition, this](Chunk* chunk) -> bool
			{
				return chunk->getPosition().distanceTo2(cameraPosition) > (renderRadius * renderRadius * 256);
			});*/

		loadedPositions = std::move(newPositions);
	}
	
	prevCameraX = cameraX;
	prevCameraY = cameraY;
	prevCameraZ = cameraZ;
}

void World::render()
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", camera.getProjectionMatrix() * camera.getViewMatrix());

	textureAtlas.bindToUnit(0);

	for (auto& it : chunks)
	{
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first));
		it.second->render();
	}

	for (auto& it : chunks)
	{
		shaderProgram.setMatrix("model", Matrix::GetTranslation(it.first));
		it.second->renderTransparent();
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
	printf("%d %d %d; %d %d %d\n", chunkX, chunkY, chunkZ, localX, localY, localZ);
	if (chunk == nullptr) return {};
	return chunk->getBlockAt(localX, localY, localZ);
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

std::vector<Vector> World::getChunkPositionsForPosition(const Vector& position)
{
	//return { {0, 0, 0, 0} };
	std::vector<Vector> positions;

	for (float x = position.x - renderRadius; x <= position.x + renderRadius; x++)
	{
		for (float y = 0; y <= 8; y++)
		{
			for (float z = position.z - renderRadius; z <= position.z + renderRadius; z++)
			{
				float xCoord = x - position.x;
				float zCoord = z - position.z;

				if (pow(xCoord, 2) + pow(zCoord, 2) <= pow(renderRadius, 2))
				{
					positions.push_back({ x * 16, y * 16, z * 16 });
				}
			}
		}
	}

	return positions;
}

//std::vector<Vector> World::getChunkPositionsToUnload(const std::vector<Vector> oldPositions, const std::vector<Vector> newPositions)
//{
//	//return {};
//	std::vector<Vector> positions;
//
//	for (const Vector& oldPosition : oldPositions)
//	{
//		if (std::find(newPositions.begin(), newPositions.end(), oldPosition) == newPositions.end())
//		{
//			positions.push_back(oldPosition);
//		}
//	}
//
//	return positions;
//}
//
std::vector<Vector> World::getChunkPositionToLoad(const std::vector<Vector> newPositions)
{
	//return {};
	std::vector<Vector> positions;

	for (const Vector& newPosition : newPositions)
	{
		if (std::find(loadedPositions.begin(), loadedPositions.end(), newPosition) == loadedPositions.end())
		{
			positions.push_back(newPosition);
		}
	}

	return positions;
}