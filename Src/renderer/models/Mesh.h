#pragma once

#include <vector>

#include "renderer/models/MeshVertex.h"
#include "renderer/wrappers/Buffer.h"
#include "renderer/wrappers/VertexArray.h"

class Mesh
{
public:
	Mesh() = default;
	Mesh(std::vector<MeshVertex>&& vertices, std::vector<unsigned int>&& indices);
	Mesh(Mesh&& other) noexcept;

	Mesh& operator=(Mesh&& other) noexcept;

	void addMesh(const Mesh& otherMesh);

	void generateBuffers();
	void render() const;
	void clear();
private:
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;

	VertexArray vertexArray;
	Buffer vertexBuffer, indexBuffer;

	Mesh(const Mesh& other) = delete;
	void operator=(const Mesh& other) = delete;
};