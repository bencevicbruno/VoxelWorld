#include "math/OctaveNoiseGenerator.h"

#include <cstdlib>
#include <ctime>

OctaveNoiseGenerator::OctaveNoiseGenerator() : OctaveNoiseGenerator(8, 0.5) {}

OctaveNoiseGenerator::OctaveNoiseGenerator(unsigned int octaves, double persistence, double constant) :
	OctaveNoiseGenerator(octaves, persistence, constant, constant, constant)
{}

OctaveNoiseGenerator::OctaveNoiseGenerator(unsigned int octaves, double persistence, double constantX, double constantY, double constantZ) :
	seed(0),
	constantX(constantX),
	constantY(constantY),
	constantZ(constantZ)
{
	std::srand((unsigned int)std::time(NULL));
	unsigned int randomSeed = (unsigned int)rand();

	this->seed = randomSeed;
	this->octaves = octaves;
	this->persistence = persistence;
	this->perlinNoise = siv::PerlinNoise(randomSeed);
}

OctaveNoiseGenerator::OctaveNoiseGenerator(unsigned int seed, unsigned int octaves, double persistence, double constant) :
	OctaveNoiseGenerator(seed, octaves, persistence, constant, constant, constant)
{}

OctaveNoiseGenerator::OctaveNoiseGenerator(unsigned int seed, unsigned int octaves, double persistence, double constantX, double constantY, double constantZ) :
	seed(seed),
	octaves(octaves),
	persistence(persistence),
	constantX(constantX),
	constantY(constantY),
	constantZ(constantZ),
	perlinNoise(siv::PerlinNoise(seed)) {}

double OctaveNoiseGenerator::get(double x, double y) const
{
	return perlinNoise.normalizedOctave2D(constantX * x, constantY * y, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get(int x, int y) const
{
	return perlinNoise.normalizedOctave2D(constantX * x, constantY * y, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get01(double x, double y) const
{
	return perlinNoise.normalizedOctave2D_01(constantX * x, constantY * y, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get01(int x, int y) const
{
	return perlinNoise.normalizedOctave2D_01(constantX * x, constantY * y, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get(double x, double y, double z) const
{
	return perlinNoise.normalizedOctave3D(constantX * x, constantY * y, constantZ * z, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get(int x, int y, int z) const
{
	return perlinNoise.normalizedOctave3D(constantX * x, constantY * y, constantZ * z, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get01(double x, double y, double z) const
{
	return perlinNoise.normalizedOctave3D_01(constantX * x, constantY * y, constantZ * z, this->octaves, this->persistence);
}

double OctaveNoiseGenerator::get01(int x, int y, int z) const
{
	return perlinNoise.normalizedOctave3D_01(constantX * x, constantY * y, constantZ * z, this->octaves, this->persistence);
}
