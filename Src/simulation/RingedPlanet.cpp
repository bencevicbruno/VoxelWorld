#include "simulation/RingedPlanet.h"

#include "math/Math.h"
#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

RingedPlanet::RingedPlanet() :
	Planet(),
	ringAngleOffset(0.0f)
{}

RingedPlanet::RingedPlanet(std::string name, float radius, float mass, float rotationSpeed, 
	float revolutionSpeed, float distanceFromSun, float innerRingRadius, float outerRingRadius) :
	Planet(name, radius, mass, rotationSpeed, revolutionSpeed, distanceFromSun),
	ringAngleOffset(Math::CreateRandom(10.0f, 60.0f)),
	ringModel(DiscModel(radius + innerRingRadius, radius + outerRingRadius, 20)),
	ringTexture(Texture::CreateFromImage("planet/" + name + "_rings.png"))
{}

RingedPlanet::RingedPlanet(RingedPlanet&& other) noexcept
{
	*this = std::move(other);
}

RingedPlanet& RingedPlanet::operator=(RingedPlanet&& other) noexcept
{
	if (this == &other) return *this;

	Planet::operator=(std::move(other));
	ringAngleOffset = std::exchange(other.ringAngleOffset, 0.0f);
	ringModel = std::move(other.ringModel);
	ringTexture = std::move(other.ringTexture);

	return *this;
}

void RingedPlanet::update(float deltaTime)
{
	Planet::update(deltaTime);
}

void RingedPlanet::render() const
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("object");

	shaderProgram.use();
	shaderProgram.setMatrix("projection_view", camera.getProjectionMatrix() * camera.getViewMatrix());
	shaderProgram.setVector("view_position", camera.getPosition());
	shaderProgram.setColor("highlight", this->isSelected() ? 
		Color::CreateFromRGB(128, 128, 128) : Color::CreateFromRGB(0, 0, 0));

	Vector position = Vector::FromPolar(distanceFromSun, revolutionAngle, 0);
	Matrix translation = Matrix::GetTranslation(position.x, position.y, position.z);
	Matrix rotation = Matrix::RotateY(rotationAngle);
	Matrix ringOff = Matrix::RotateX(ringAngleOffset);
	shaderProgram.setMatrix("model", translation * rotation * ringOff);

	texture.bindToUnit(0);
	model.render();


	shaderProgram.setMatrix("model", translation * rotation * ringOff);

	ringTexture.bindToUnit(0);
	ringModel.render();
}