#pragma once

#include <string>

#include "renderer/wrappers/Texture3D.h"
#include "renderer/models/SkyboxModel.h"

class Skybox
{
public:
	Skybox();
	Skybox(const Color& color);
	Skybox(const std::string& name);
	Skybox(Skybox&& other) noexcept;

	Skybox& operator=(Skybox&& other) noexcept;

	void render();
private:
	Texture3D texture;
	SkyboxModel model;

	Skybox(const Skybox& other) = delete;
	void operator=(const Skybox& other) = delete;
};
