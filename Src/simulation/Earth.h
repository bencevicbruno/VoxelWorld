#pragma once

#include "simulation/Planet.h"

class Earth : public Planet
{
public:
	Earth();
	Earth(Earth&& other) noexcept;
	Earth& operator=(Earth&& other) noexcept;

	void update(float deltaTime) override;
	void render() const override;

private:
	Texture nightTexture;
	Texture cloudsTexture;
	Texture specularTexture;
};