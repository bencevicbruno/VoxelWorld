#include "renderer/gui/SpeedButton.h"

#include "devices/Cursor.h"
#include "devices/Window.h"

#include "renderer/shader/ShaderProgramRegistry.h"

SpeedButton::SpeedButton() :
	x(0), y(0),
	speedFactor(0.0f),
	selected(false)
{}

SpeedButton::SpeedButton(const std::string& name, float speedFactor, int x, int y) :
	x(x), y(y),
	texture(Texture::CreateFromImage("gui/icon_" + name + ".png")),
	textureHovered(Texture::CreateFromImage("gui/icon_" + name + "_hovered.png")),
	textureSelected(Texture::CreateFromImage("gui/icon_" + name + "_selected.png")),
	speedFactor(speedFactor),
	selected(name == "speed_1x"),
	vertexArray(VertexArray::CreateForGUI())
{
	vertexArray.bind();

	setupVertexBuffer();
	setupIndexBuffer();

	vertexArray.setupAttributeData();
}

SpeedButton::SpeedButton(SpeedButton&& other) noexcept
{
	*this = std::move(other);
}

SpeedButton& SpeedButton::operator=(SpeedButton&& other) noexcept
{
	if (this == &other) return *this;

	x = std::exchange(other.x, 0);
	y = std::exchange(other.y, 0);
	texture = std::move(other.texture);
	textureHovered = std::move(other.textureHovered);
	textureSelected = std::move(other.textureSelected);
	speedFactor = std::exchange(other.speedFactor, 0.0f);
	selected = std::exchange(other.selected, false);
	vertexArray = std::move(other.vertexArray);
	vertexBuffer = std::move(other.vertexBuffer);
	indexBuffer = std::move(other.indexBuffer);

	return *this;
}

void SpeedButton::select()
{
	selected = true;
}

void SpeedButton::deselect()
{
	selected = false;
}

void SpeedButton::render() const
{
	const ShaderProgram& shader = ShaderProgramRegistry::GetInstance().getShaderProgram("gui");
	shader.use();

	Matrix scale = Matrix::GetScaling((float)SIZE, (float)SIZE, 0.0f);
	Matrix translate = Matrix::GetTranslation((float)x, (float)y, 0.0f);
	Matrix model = translate * scale;

	const Texture* textureToUse = nullptr;
	if (selected)
	{
		textureToUse = &textureSelected;
	}
	else
	{
		textureToUse = isHoveredOn() ? &textureHovered : &texture;
	}

	shader.setMatrix("model", model);
	shader.setMatrix("projection", Window::GetInstance().getOrthogonalMatrix());
	textureToUse->bindToUnit(0);

	vertexArray.bind();
	vertexBuffer.bind();
	indexBuffer.bind();

	glDrawElements(GL_TRIANGLES, indexBuffer.getCount(), GL_UNSIGNED_INT, 0);
}

void SpeedButton::onClick()
{
	simulationSpeedFactor = speedFactor;
	select();
}

bool SpeedButton::isHoveredOn() const
{
	const Cursor& cursor = Cursor::GetInstance();
	int cursorX = cursor.getX();
	int cursorY = cursor.getY();

	return cursorX >= x && cursorX <= (x + SIZE) &&
		cursorY >= y && cursorY <= (y + SIZE);
}

float SpeedButton::simulationSpeedFactor = 10e-6f;

float SpeedButton::getSimulationSpeedFactor()
{
	return simulationSpeedFactor;
}

void SpeedButton::setupVertexBuffer()
{
	std::vector<float> vertices = {
		0.0f, 0.0f, 0.0f,	0.0f, 1.0f, // top left
		1.0f, 0.0f, 0.0f,	1.0f, 1.0f, // top right
		0.0f, 1.0f, 0.0f,	0.0f, 0.0f, // bottom left
		1.0f, 1.0f, 0.0f,	1.0f, 0.0f }; // bottom right

	vertexBuffer = Buffer::CreateVertex(vertices);
	vertexBuffer.bind();
}

void SpeedButton::setupIndexBuffer()
{
	indexBuffer = Buffer::CreateIndex({ 2, 3, 1, 2, 1, 0 });
	indexBuffer.bind();
}