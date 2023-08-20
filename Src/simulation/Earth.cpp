#include "simulation/Earth.h"

#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

Earth::Earth():
	Planet("earth", 1.0f, 100.f, 0.5f, 0.01f, 30.0f),
	nightTexture(Texture::CreateFromImage("planet/earth_night.png")),
	cloudsTexture(Texture::CreateFromImage("planet/earth_clouds.png")),
	specularTexture(Texture::CreateFromImage("planet/earth_specular.png"))
{}

Earth::Earth(Earth&& other) noexcept
{
	*this = std::move(other);
}

Earth& Earth::operator=(Earth&& other) noexcept
{
	if (this == &other) return *this;

	Planet::operator=(std::move(other));
	nightTexture = std::move(other.nightTexture);
	cloudsTexture = std::move(other.cloudsTexture);
	specularTexture = std::move(other.specularTexture);

	return *this;
}

void Earth::update(float deltaTime)
{
	Planet::update(deltaTime);
}

void Earth::render() const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("earth");

	Matrix projection = camera.getProjectionMatrix();
	Matrix view = camera.getViewMatrix();
	Vector position = Vector::FromPolar(distanceFromSun, revolutionAngle, 0);
	Matrix model = Matrix::GetTranslation(position.x, position.y, position.z) * Matrix::RotateY(rotationAngle);

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", projection * view);
	shaderProgram.setMatrix("model", model);
	shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setColor("highlight", this->isSelected() ? Color::CreateFromRGB(128, 128, 128) : Color::CreateFromRGB(0, 0, 0));

	shaderProgram.setInt("earth_texture", 0);
	shaderProgram.setInt("clouds_texture", 1);
	shaderProgram.setInt("night_texture", 2);
	shaderProgram.setInt("specular_texture", 3);

	texture.bindToUnit(0);
	cloudsTexture.bindToUnit(1);
	nightTexture.bindToUnit(2);
	specularTexture.bindToUnit(3);

	this->model.render();
}