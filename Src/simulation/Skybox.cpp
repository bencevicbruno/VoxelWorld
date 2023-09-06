#include "simulation/Skybox.h"

#include "renderer/camera/Camera.h"
#include "renderer/shader/ShaderProgramRegistry.h"

Skybox::Skybox()
{}

Skybox::Skybox(const Color& color) :
	model(SkyboxModel(1.0f)),
	texture(Texture3D(color))
{}

Skybox::Skybox(const std::string& name) :
	model(SkyboxModel(1.0f)),
	texture(Texture3D("skybox/" + name))
{}

Skybox::Skybox(Skybox&& other) noexcept
{
	*this = std::move(other);
}

Skybox& Skybox::operator=(Skybox&& other) noexcept
{
	if (this == &other) return *this;

	model = std::move(other.model);
	texture = std::move(other.texture);

	return *this;
}

void Skybox::render()
{
	const Camera& camera = Camera::getActiveCamera();
	const ShaderProgram& shaderProgram = ShaderProgramRegistry::GetInstance().getShaderProgram("skybox");

	shaderProgram.use();
	shaderProgram.setInt("skybox_texture", 0);
	shaderProgram.setMatrix("projection", camera.getProjectionMatrix());
	shaderProgram.setMatrix("view", camera.getViewMatrix());

	texture.bind();
	model.render();
}