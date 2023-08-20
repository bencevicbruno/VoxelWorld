#include "simulation/Sun.h"

#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

Sun::Sun():
	radius(10.0f),
	mass(3330542.53f),
	model(SphereModel(radius, 51, 0.0f, 0.0f)),
	texture(Texture::CreateFromImage("planet/sun.png"))
{}

Sun::Sun(Sun&& other) noexcept
{
	*this = std::move(other);
}

Sun& Sun::operator=(Sun&& other) noexcept
{
	if (this == &other) return *this;

	radius = std::exchange(other.radius, 0.0f);
	mass = std::exchange(other.mass, 0.0f);
	model = std::move(other.model);
	texture = std::move(other.texture);

	return *this;
}

void Sun::render() const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("sun");

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", 
		camera.getProjectionMatrix() * camera.getViewMatrix());

	texture.bindToUnit(0);
	model.render();
}

float Sun::getRadius() const
{
	return radius;
}

float Sun::getMass() const
{
	return mass;
}