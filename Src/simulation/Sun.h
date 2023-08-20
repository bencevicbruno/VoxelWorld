#pragma once

#include "renderer/models/SphereModel.h"
#include <renderer/wrappers/Texture.h>

class Sun
{
public:
	Sun();
	Sun(Sun&& other) noexcept;

	Sun& operator=(Sun&& other) noexcept;

	void render() const;

	float getRadius() const;
	float getMass() const;

private:
	float radius;
	float mass;
	SphereModel model;
	Texture texture;

	Sun(const Sun& other) = delete;
	void operator=(const Sun& other) = delete;
};