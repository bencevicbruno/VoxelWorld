#pragma once

#pragma warning(push, 0)
#pragma warning( disable : 26812)
#include "FastNoiseLite.h"
#pragma warning(pop)

class PerlinNoiseGenerator
{
public:
	PerlinNoiseGenerator();
	PerlinNoiseGenerator(unsigned int seed, double constant = 1);
	PerlinNoiseGenerator(unsigned int seed, FastNoiseLite::NoiseType noiseType, double constant = 1);
	PerlinNoiseGenerator(unsigned int seed, double constanX, double constantY, double constantZ);
	PerlinNoiseGenerator(unsigned int seed, FastNoiseLite::NoiseType noiseType, double constanX, double constantY, double constantZ);

	double get(double x, double y) const;
	double get(int x, int y) const;
	double get(double x, double y, double z) const;
	double get(int x, int y, int z) const;

	static double get01(unsigned int seed, double x, double y, double z);
private:
	unsigned int seed;
	double constantX, constantY, constantZ;

	FastNoiseLite perlinNoise;
};