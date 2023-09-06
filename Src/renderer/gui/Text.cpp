#include <renderer/gui/Text.h>

#include <cmath>
#include <vector>

#include "devices/Window.h"
#include "opengl.h"
#include "renderer/font/FontManager.h"
#include "renderer/shader/ShaderProgramRegistry.h"

const float Text::SCALE = 5.0f;

Text::Text() :
	text(""),
	x(0),
	y(0),
	width(0),
	height(0),
	noOfDrawableChars(0)
{}

Text::Text(std::string text_, int x, int y) :
	x(x),
	y(y),
	vertexArray(VertexArray::CreateForGUI()),
	noOfDrawableChars(0)
{
	text_[0] = text_[0] & ~0b0010'0000;
	text = std::move(text_);

	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();
}

Text::Text(Text&& other) noexcept
{
	*this = std::move(other);
}

Text& Text::operator=(Text&& other) noexcept
{
	if (this == &other) return *this;

	text = std::move(other.text);
	x = std::exchange(other.x, 0);
	y = std::exchange(other.y, 0);
	width = std::exchange(other.width, 0);
	height = std::exchange(other.height, 0),
		noOfDrawableChars = std::exchange(other.noOfDrawableChars, 0);

	return *this;
}

void Text::render() const
{
	const Window& window = Window::GetInstance();
	const ShaderProgram& shader = ShaderProgramRegistry::GetInstance().getShaderProgram("gui");
	shader.use();

	Matrix scale = Matrix::GetScaling(SCALE, SCALE * 1600.0f / 900.0f, 0);
	Matrix translate = Matrix::GetTranslation((float)x, (float)y, 0.0f);
	Matrix model = translate * scale;

	shader.setMatrix("model", model);
	shader.setMatrix("projection", window.getOrthogonalMatrix());
	FontManager::GetInstance().getTexture().bindToUnit(0);

	vertexArray.bind();
	vertexBuffer.bind();
	indexBuffer.bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
}

void Text::centerHorizontally()
{
	x = (int)(Window::WIDTH - SCALE * width) / 2;
}

void Text::setY(int newY)
{
	y = newY;
}

unsigned int Text::getHeight() const
{
	return height;
}

int Text::getY() const
{
	return y;
}

void Text::setupVertexBuffer()
{
	std::vector<float> vertices;

	float offset = 0.0f;
	noOfDrawableChars = 0;

	for (unsigned char letter : text)
	{
		if (letter == ' ')
		{
			offset += 2;
			continue;
		}

		noOfDrawableChars++;

		const Glyph& glyph = FontManager::GetInstance().getGlpyh(letter);
		const float width = (float)glyph.getWidth();
		const float height = (float)glyph.getHeight();

		vertices.insert(vertices.end(), { // top-left
			offset + 0.0f, 16.0f - height, 0.0f, glyph.getMinU(), glyph.getMaxV() });
		vertices.insert(vertices.end(), { // top-right
			offset + width, 16.0f - height, 0.0f, glyph.getMaxU(), glyph.getMaxV() });
		vertices.insert(vertices.end(), { // bottom-left
			offset + 0.0f, 16.0f - 0.0f, 0.0f, glyph.getMinU(), glyph.getMinV() });
		vertices.insert(vertices.end(), { // bottom-right
			offset + width, 16.0f - 0.0f, 0.0f, glyph.getMaxU(), glyph.getMinV() });

		offset += width + 1;
		this->height = std::max(this->height, (unsigned int)height);
	}

	width = (int)(offset - 1.0f);

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

void Text::setupIndexBuffer()
{
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < noOfDrawableChars; i++)
	{
		indices.insert(indices.end(), {
			4 * i + 2, 4 * i + 3, 4 * i + 1,
			4 * i + 2, 4 * i + 1, 4 * i + 0
			});
	}

	indexBuffer = Buffer::CreateIndex(indices);
	indexBuffer.bind();
}