#include "world/terrain/MinecraftTerrainGenerator.h"

#include <iostream>

#include "math/Math.h"
#include "utils/debug/Debug.h"
#include "world/chunk/Chunk.h"
#include "Options.h"

FinalTerrainGenerator::FinalTerrainGenerator(unsigned int seed) :
	seaBed(16),
	continentalnessGenerator(PerlinNoiseGenerator(seed, 0.8)),
	continentalnessInterpolator(LinearInterpolator({
	//{0.0, 0.0}, {0.5, 0.5}, {1.0, 0.0}
	//{0.0, 1.0}, {0.15, 0.1}, {0.25, 0.15}, {1.0, 0.4}
	{0.0, 0.0}, {0.25, 0.2}, {0.65, 0.33}, {0.85, 0.4}, {1.0, 1.0}
		})),
	erosionGenerator(PerlinNoiseGenerator(seed << 4, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 0.5, 1, 0.5)),
	erosionInterpolator(LinearInterpolator({
		{0.0, -0.25}, {0.5, 0}, {1.0, 0.25}
		})),
	surfaceCaveNoiseGenerator(PerlinNoiseGenerator(seed * 5, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 2.3, 0.9, 2.3)),
	undergroundCaveNoiseGenerator(PerlinNoiseGenerator(seed * 10, FastNoiseLite::NoiseType::NoiseType_OpenSimplex2, 0.8))
{}

unsigned char* FinalTerrainGenerator::generateTerrain(unsigned int seed, const Vector& position, const int* heightMap) const
{
	unsigned char* blocks = new unsigned char[CHUNK_WIDTH * CHUNK_WIDTH * CHUNK_HEIGHT];

	for (int x = 0; x < CHUNK_WIDTH; x++)
	{
		int globalX = int(16 * position.x + x);

		for (int z = 0; z < CHUNK_WIDTH; z++)
		{
			int globalZ = int(16 * position.z + z);

			int height = heightMap[x * CHUNK_WIDTH + z];

			for (int y = 0; y < CHUNK_HEIGHT; y++)
			{
				int globalY = int(position.y + y);

				double caveValue = surfaceCaveNoiseGenerator.get(globalX, globalY, globalZ);
				bool isPartOfSurfaceCave = (caveValue > 0.9) && (globalY <= height);

				double undergroundCaveValue = undergroundCaveNoiseGenerator.get(globalX, globalY, globalZ);
				bool isPartOfUndergroundCave = (undergroundCaveValue > 0.3) && (undergroundCaveValue < 0.45) && (globalY <= height - 10);


				bool isBlock = globalY <= height
					&& !isPartOfSurfaceCave
					&& !isPartOfUndergroundCave;

				if (!ENABLE_CAVES)
				{
					isBlock = globalY <= height;
				}

				blocks[Chunk::coordsToOffset(x, y, z)] = isBlock ? BLOCK_STONE : BLOCK_AIR;
			}
		}
	}

	return blocks;
}

int* FinalTerrainGenerator::generateHeightMap(int x, int z)
{
	int maxHeight = CHUNK_HEIGHT - seaBed;
	int* heightMap = new int[CHUNK_WIDTH * CHUNK_WIDTH];

	for (int localX = 0; localX < 16; localX++)
	{
		for (int localZ = 0; localZ < 16; localZ++)
		{
			double continentalness = continentalnessGenerator.get(16 * x + localX, 16 * z + localZ);
			double continentalnessFactor = continentalnessInterpolator.interpolate(continentalness);

			double erosion = erosionGenerator.get(16 * x + localX, 16 * z + localZ);
			double erosionFactor = erosionInterpolator.interpolate(erosion);

			heightMap[CHUNK_WIDTH * localX + localZ] = Math::Clamp(seaBed + int((continentalnessFactor + erosionFactor) * maxHeight), 0, CHUNK_HEIGHT - 1);
		}
	}

	return heightMap;
}