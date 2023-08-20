#pragma once

struct MeshVertex
{
	float x, y, z;
	float u, v;
	float nx, ny, nz;
	float r, g, b, a;

	static const unsigned int SIZE = 12;
};

struct UVPosition
{
	float minU, minV, maxU, maxV;

	static UVPosition FromAtlas(int xPosition, int yPosition)
	{
		return { xPosition / 16.0f, 1 - yPosition / 16.0f, (xPosition + 1) / 16.0f, 1 - (yPosition + 1) / 16.0f };
	}
};