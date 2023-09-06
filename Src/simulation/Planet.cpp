#include "simulation/Planet.h"

#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

Planet::Planet() :
	name(""),
	radius(0.0f),
	mass(0.0f),
	rotationSpeed(0.0f),
	revolutionSpeed(0.0f),
	distanceFromSun(0.0f),
	rotationAngle(0.0f),
	revolutionAngle(0.0f),
	revolutionPitch(0.0f),
	selected(false)
{}

Planet::Planet(std::string name, float radius, float mass, float rotationSpeed, float revolutionSpeed, float distanceFromSun) :
	Planet(name, radius, mass, rotationSpeed, revolutionSpeed, distanceFromSun, Math::CreateRandom(0.0f, 360.0f), 0.0f, 0.0f,
		Texture::CreateFromImage("planet/" + name + ".png"), SphereModel(radius, 30, 0.0f, 0.0f))
{}

Planet::Planet(std::string name, float radius, float mass, float rotationSpeed, float revolutionSpeed,
	float distanceFromSun, float rotationAngle, float revolutionAngle, float revolutionPitch, Texture texture, SphereModel model) :
	name(name),
	radius(radius),
	mass(mass),
	rotationSpeed(rotationSpeed),
	revolutionSpeed(revolutionSpeed),
	distanceFromSun(distanceFromSun),
	rotationAngle(rotationAngle),
	revolutionAngle(revolutionAngle),
	revolutionPitch(revolutionPitch),
	model(std::move(model)),
	texture(std::move(texture)),
	selected(false)
{}

Planet::Planet(Planet&& other) noexcept
{
	*this = std::move(other);
}

Planet& Planet::operator=(Planet&& other) noexcept
{
	if (this != &other)
	{
		name = std::move(other.name);
		radius = std::exchange(other.radius, 0.0f);
		mass = std::exchange(other.mass, 0.0f);
		rotationSpeed = std::exchange(other.rotationSpeed, 0.0f);
		revolutionSpeed = std::exchange(other.revolutionSpeed, 0.0f);
		distanceFromSun = std::exchange(other.distanceFromSun, 0.0f);
		rotationAngle = std::exchange(other.rotationAngle, 0.0f);
		revolutionAngle = std::exchange(other.revolutionAngle, 0.0f);
		model = std::move(other.model);
		texture = std::move(other.texture);
		selected = std::exchange(other.selected, false);
	}

	return *this;
}

std::string Planet::getName() const
{
	return name;
}

float Planet::getRadius() const
{
	return radius;
}

float Planet::getMass() const
{
	return mass;
}

Vector Planet::getPosition() const
{
	return Vector::FromPolar(distanceFromSun, revolutionAngle, revolutionPitch);
}

void Planet::update(float deltaTime)
{
	revolutionAngle += revolutionSpeed * deltaTime;
	rotationAngle += rotationSpeed * deltaTime;

	float pitch = (float)std::sin(revolutionAngle * 0.01);
	pitch *= 80;

	revolutionPitch = pitch;
}

void Planet::render() const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	const Color colorSelected = Color::CreateFromRGB(128, 128, 128);

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", camera.getProjectionMatrix() * camera.getViewMatrix());
	shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setColor("highlight", this->isSelected() ? colorSelected : Color::CreateFromRGB(0, 0, 0));

	Vector position = getPosition();

	Matrix modelMatrix =
		Matrix::GetTranslation(position.x, position.y, position.z)
		* Matrix::RotateY(rotationAngle);
	shaderProgram.setMatrix("model", modelMatrix);

	texture.bindToUnit(0);
	model.render();
}

/* ISelectable */

void Planet::select()
{
	this->selected = true;
}

void Planet::deselect()
{
	this->selected = false;
}

bool Planet::isSelected() const
{
	return this->selected;
}

/* static methods */

Planet* Planet::CreateRandom()
{
	std::string name = "planet #" + std::to_string(rand());
	float radius = Math::CreateRandom() * 4.0f + 1.0f;
	float mass = radius * 100;
	float rotationSpeed = Math::CreateRandom(0.5f, 1.5f);
	float revolutionSpeed = Math::CreateRandom(0.1f, 0.5f);
	float distanceFromSun = Math::CreateRandom(120.0f, 1000.0f);
	float revolutionAngle = Math::CreateRandom(0.0f, 360.0f);
	float revolutionPitch = Math::CreateRandom(-90.0f, 90.f);

	Texture texture = Texture::CreateFromColor(Color::CreateRandom());
	SphereModel model = SphereModel(radius, 30, 0.0f, 10.0f);

	return new Planet(name, radius, mass, rotationSpeed, revolutionSpeed,
		distanceFromSun, 0.0f, revolutionAngle, revolutionPitch, std::move(texture), std::move(model));
}