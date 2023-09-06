#include "renderer/models/BlockModel.h"

#include "devices/Window.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"
#include "utils/image/ImageManager.h"

BlockModel::BlockModel(const std::string& texture) :
	BaseModel(VertexArray::CreateForWorld()),
	texture(Texture::CreateFromImage(ImageManager::GetInstance().getImage(texture)))
{
	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();

	this->texture.setWrapAndFilterForGUI();
}

void BlockModel::setupVertexBuffer()
{
	float size = 1.0f;

	std::vector<float> vertices =
	{
		// BOTTOM
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f, -1.0f, 1.0f,     0.4f, 0.4f, 0.4f, 1.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   1.0f, -1.0f, 1.0f,     0.4f, 0.4f, 0.4f, 1.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 1.0f,   1.0f, -1.0f, 1.0f,     0.4f, 0.4f, 0.4f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,   1.0f, -1.0f, 1.0f,     0.4f, 0.4f, 0.4f, 1.0f,

		 // TOP
		 -0.5f, 0.5f, -0.5f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
		  0.5f, 0.5f, -0.5f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
		  0.5f, 0.5f,  0.5f,  1.0f, 1.0f,   1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,
		 -0.5f, 0.5f,  0.5f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f,     1.0f, 1.0f, 1.0f, 1.0f,

		 // NORTH FACE
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,

		// SOUTH FACE
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.6f, 0.6f, 0.6f, 1.0f,

		// west face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,

		// east face
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 1.0f, 1.0f,     0.8f, 0.8f, 0.8f, 1.0f,
	};

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

void BlockModel::setupIndexBuffer()
{
	std::vector<unsigned int> indices;

	indices.insert(indices.end(), {
		0, 1, 2, 0, 2, 3, // bottom
		4 * 1 + 0, 4 * 1 + 2, 4 * 1 + 1, 4 * 1 + 0, 4 * 1 + 3, 4 * 1 + 2, // top
		4 * 2 + 0, 4 * 2 + 2, 4 * 2 + 1, 4 * 2 + 0, 4 * 2 + 3, 4 * 2 + 2, // north
		4 * 3 + 0, 4 * 3 + 1, 4 * 3 + 2, 4 * 3 + 0, 4 * 3 + 2, 4 * 3 + 3, // south
		4 * 4 + 0, 4 * 4 + 1, 4 * 4 + 2, 4 * 4 + 0, 4 * 4 + 2, 4 * 4 + 3, // west
		4 * 5 + 0, 4 * 5 + 2, 4 * 5 + 1, 4 * 5 + 0, 4 * 5 + 3, 4 * 5 + 2 // east
		});

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();
}

void BlockModel::render(double x, double y, double z) const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", camera.getProjectionMatrix() * camera.getViewMatrix());
	//shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setMatrix("model", Matrix::GetTranslation(x, y, z));

	texture.bindToUnit(0);

	vertexArray.bind();
	vertexBuffer.bind();
	indexBuffer.bind();
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
}