#include "simulation/Asteroid.h"

#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

Asteroid::Asteroid():
	name("Asteroid #" + std::to_string(std::rand())),
	radius(Math::CreateRandom(0.1f, 0.5f)),
	position(Vector::CreateRandom(-300.0f, 300.0f)),
	velocity({}),
	acceleration({}),
	texture(Texture::CreateFromImage("asteroid.png")),
	model(SphereModel(radius, (unsigned int) Math::CreateRandom(20, 30), Math::CreateRandom(0.8f, 1.2f), Math::CreateRandom(15.0f, 20.0f))),
	selected(false)
{}

Asteroid::Asteroid(Asteroid&& other) noexcept
{
	*this = std::move(other);
}

Asteroid& Asteroid::operator=(Asteroid&& other) noexcept
{
	if (this == &other) return *this;

	name = std::move(other.name);
	radius = other.radius;
	position = other.position;
	velocity = other.velocity;
	acceleration = other.acceleration;
	texture = std::move(other.texture);
	model = std::move(other.model);
	selected = std::exchange(other.selected, false);

	return *this;
}

Vector Asteroid::getPosition() const
{
	return position;
}

float Asteroid::getRadius() const
{
	return radius;
}

void Asteroid::update(float deltaTime, const Vector& gravitationalForce)
{
	acceleration = gravitationalForce;
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;
}

void Asteroid::render() const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	const Color colorSelected = Color::CreateFromRGB(128, 128, 128);

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", camera.getProjectionMatrix() * camera.getViewMatrix());
	shaderProgram.setMatrix("model", Matrix::GetTranslation(position.x, position.y, position.z));
	shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setColor("highlight", this->isSelected() ? colorSelected : Color::CreateFromRGB(0, 0, 0));

	texture.bindToUnit(0);
	model.render();
}

void Asteroid::select()
{
	this->selected = true;
}

void Asteroid::deselect()
{
	this->selected = false;
}

bool Asteroid::isSelected() const
{
	return this->selected;
}

std::string Asteroid::getName() const
{
	return name;
}