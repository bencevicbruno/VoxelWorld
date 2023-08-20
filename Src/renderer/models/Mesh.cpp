#include "renderer/models/Mesh.h"

#include "opengl.h"

Mesh::Mesh(std::vector<MeshVertex>&& vertices, std::vector<unsigned int>&& indices) :
	vertices(vertices),
	indices(indices)
{}

Mesh::Mesh(Mesh&& other) noexcept
{
	*this = std::move(other);
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
	if (this == &other) return *this;

	vertices = std::move(other.vertices);
	indices = std::move(other.indices);

	vertexArray = std::move(other.vertexArray);
	vertexBuffer = std::move(other.vertexBuffer);
	indexBuffer = std::move(other.indexBuffer);

	return *this;
}

void Mesh::addMesh(const Mesh& otherMesh)
{
	const unsigned int verticesCount = vertices.size();

	vertices.insert(vertices.end(), otherMesh.vertices.begin(), otherMesh.vertices.end());


	for (unsigned int index : otherMesh.indices)
	{
		indices.push_back(verticesCount + index);
	}
}

void Mesh::generateBuffers()
{
	vertexArray = VertexArray::CreateForWorld();
	vertexArray.bind();

	vertexBuffer = Buffer::CreateVertex((float*) vertices.data(), vertices.size() * MeshVertex::SIZE);
	vertexBuffer.bind();

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();

	vertexArray.setupAttributeData();
}

void Mesh::render() const
{
	vertexArray.bind();
	vertexBuffer.bind();
	indexBuffer.bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
}