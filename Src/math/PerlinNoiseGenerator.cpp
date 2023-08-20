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
	this->perlinNoise = siv::PerlinNoise(randomSeed);
}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, double constant) :
	seed(seed),
	perlinNoise(siv::PerlinNoise(seed)),
	constantX(constant),
	constantY(constant),
	constantZ(constant)
{}

PerlinNoiseGenerator::PerlinNoiseGenerator(unsigned int seed, double constantX, double constantY, double constantZ) :
	seed(seed),
	perlinNoise(siv::PerlinNoise(seed)),
	constantX(constantX),
	constantY(constantY),
	constantZ(constantZ)
{}

double PerlinNoiseGenerator::get(double x, double y) const
{
	return perlinNoise.noise2D(constantX * x, constantY * y);
}

double PerlinNoiseGenerator::get(int x, int y) const
{
	return perlinNoise.noise2D(constantX * x, constantY * y);
}

double PerlinNoiseGenerator::get01(double x, double y) const
{
	return perlinNoise.noise2D_01(constantX * x, constantY * y);
}

double PerlinNoiseGenerator::get01(int x, int y) const
{
	return perlinNoise.noise2D_01(constantX * x, constantY * y);
}

double PerlinNoiseGenerator::get(double x, double y, double z) const
{
	return perlinNoise.noise3D(constantX * x, constantY * y, constantZ * z);
}

double PerlinNoiseGenerator::get(int x, int y, int z) const
{
	return perlinNoise.noise3D(constantX * x, constantY * y, constantZ * z);
}

double PerlinNoiseGenerator::get01(double x, double y, double z) const
{
	return perlinNoise.noise3D_01(constantX * x, constantY * y, constantZ * z);
}

double PerlinNoiseGenerator::get01(int x, int y, int z) const
{
	return perlinNoise.noise3D_01(constantX * x, constantY * y, constantZ * z);
}

double PerlinNoiseGenerator::get01(unsigned int seed, double x, double y, double z)
{
	return siv::PerlinNoise(seed).noise3D_01(x, y, z);
}