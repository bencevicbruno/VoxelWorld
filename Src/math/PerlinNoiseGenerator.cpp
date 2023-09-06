#include "math/PerlinNoiseGenerator.h"

#include <cstdlib>
#include <ctime>

PerlinNoiseGenerator::PerlinNoiseGenerator() :
	seed(0),
	constantX(1),
	constantY(1),
	constantZ(1)
{
	std::srand((unsigned int)std::time(NULL));
	unsigned int randomSeed = (unsigned int)rand();

	this->seed = randomSeed;
	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	perlinNoise.SetSeed(randomSeed);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, double constant) :
	PerlinNoiseGenerator(seed, constant, constant, constant)
{}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, FastNoiseLite::NoiseType noiseType, double constant) :
	seed(seed),
	constantX(constant),
	constantY(constant),
	constantZ(constant)
{
	perlinNoise.SetNoiseType(noiseType);
	perlinNoise.SetSeed(seed);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, double constantX, double constantY, double constantZ) :
	seed(seed),
	constantX(constantX),
	constantY(constantY),
	constantZ(constantZ)
{
	perlinNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	perlinNoise.SetSeed(seed);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, FastNoiseLite::NoiseType noiseType, double constantX, double constantY, double constantZ) :
	seed(seed),
	constantX(constantX),
	constantY(constantY),
	constantZ(constantZ)
{
	perlinNoise.SetNoiseType(noiseType);
	perlinNoise.SetSeed(seed);
}

double PerlinNoiseGenerator::get(double x, double y) const
{
	return (perlinNoise.GetNoise(constantX * x, constantY * y) + 1.0) * 0.5;
}

double PerlinNoiseGenerator::get(int x, int y) const
{
	return get(constantX * x, constantY * y);
}

double PerlinNoiseGenerator::get(double x, double y, double z) const
{
	return (perlinNoise.GetNoise(constantX * x, constantY * y, constantZ * z) + 1.0) * 0.5;
}

double PerlinNoiseGenerator::get(int x, int y, int z) const
{
	return get(constantX * x, constantY * y, constantZ * z);
}