#pragma once

#include "Reputeless_PerlinNoise.h"

class PerlinNoiseGenerator
{
public:
	PerlinNoiseGenerator();
	PerlinNoiseGenerator(unsigned int seed, double constant = 1);
	PerlinNoiseGenerator(unsigned int seed, double constanX, double constantY, double constantZ);

	double get(double x, double y) const;
	double get(int x, int y) const;
	double get01(double x, double y) const;
	double get01(int x, int y) const;
	double get(double x, double y, double z) const;
	double get(int x, int y, int z) const;
	double get01(double x, double y, double z) const;
	double get01(int x, int y, int z) const;

	static double get01(unsigned int seed, double x, double y, double z);
private:
	unsigned int seed;
	double constantX, constantY, constantZ;

	siv::PerlinNoise perlinNoise;
};