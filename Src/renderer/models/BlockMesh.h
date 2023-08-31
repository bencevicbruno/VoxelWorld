#pragma once

#include <string>

#include "math/Vector.h"
#include "renderer/models/Mesh.h"
#include "utils/Color.h"

struct Box
{
	float minX, minY, minZ;
	float maxX, maxY, maxZ;

	Box(float min, float max) :
		minX(min), minY(min), minZ(min),
		maxX(max), maxY(max), maxZ(max)
	{};

	Box(float minX, float minY, float minZ, float maxX, float maxY, float maxZ) :
		minX(minX), minY(minY), minZ(minZ),
		maxX(maxX), maxY(maxY), maxZ(maxZ)
	{};
};

class BlockMesh
{
public:
	BlockMesh();
	BlockMesh(
		const UVPosition& texturePosition,
		const Box& box = Box(-0.5, 0.5));
	BlockMesh(
		const UVPosition& verticalTexturePosition,
		const UVPosition& sideTexturePosition,
		const Box& box = Box(-0.5, 0.5));
	BlockMesh(
		const UVPosition& topTexturePosition,
		const UVPosition& bottomTexturePosition,
		const UVPosition& sideTexturePosition,
		const Box& box = Box(-0.5, 0.5));
	BlockMesh(
		const UVPosition& topTexturePosition,
		const UVPosition& bottomTexturePosition,
		const UVPosition& northTexturePosition,
		const UVPosition& southTexturePosition,
		const UVPosition& westTexturePosition,
		const UVPosition& eastTexturePosition,
		const Box& box = Box(-0.5, 0.5));

	BlockMesh(BlockMesh&& other) noexcept = default;

	BlockMesh& operator=(BlockMesh&& other) noexcept = default;

	Mesh getBottomFace(Vector position, Color color = Color::CreateFromRGB(128, 128, 128)) const;
	Mesh getTopFace(Vector position, Color color = Color::CreateFromRGB(255, 255, 255)) const;
	Mesh getNorthFace(Vector position, Color color = Color::CreateFromRGB(200, 200, 200)) const;
	Mesh getSouthFace(Vector position, Color color = Color::CreateFromRGB(200, 200, 200)) const;
	Mesh getWestFace(Vector position, Color color = Color::CreateFromRGB(170, 170, 170)) const;
	Mesh getEastFace(Vector position, Color color = Color::CreateFromRGB(170, 170, 170)) const;

	Mesh getCrossMesh(Vector position, Color color = Color::CreateFromRGB(200, 200, 200)) const;

private:
	UVPosition topTexturePosition;
	UVPosition bottomTexturePosition;
	UVPosition northTexturePosition;
	UVPosition southTexturePosition;
	UVPosition westTexturePosition;
	UVPosition eastTexturePosition;
	Box box;

	BlockMesh(const BlockMesh& other) = delete;
	void operator=(const BlockMesh& other) = delete;
};