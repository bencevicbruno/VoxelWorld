#pragma once

#include "Reputeless_PerlinNoise.h"

class OctaveNoiseGenerator
{
public:
	OctaveNoiseGenerator();
	OctaveNoiseGenerator(unsigned int octaves, double persistence, double constant = 1);
	OctaveNoiseGenerator(unsigned int octaves, double persistence, double constantX, double constantY, double constantZ);
	OctaveNoiseGenerator(unsigned int seed, unsigned int octaves, double persistence, double constant = 1);
	OctaveNoiseGenerator(unsigned int seed, unsigned int octaves, double persistence, double constantX, double constantY, double constantZ);

	double get(double x, double y) const;
	double get(int x, int y) const;
	double get01(double x, double y) const;
	double get01(int x, int y) const;
	double get(double x, double y, double z) const;
	double get(int x, int y, int z) const;
	double get01(double x, double y, double z) const;
	double get01(int x, int y, int z) const;
private:
	unsigned int seed;
	unsigned int octaves;
	double persistence;

	double constantX, constantY, constantZ;

	siv::PerlinNoise perlinNoise;
};