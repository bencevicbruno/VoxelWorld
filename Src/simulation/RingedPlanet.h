#pragma once

#include "renderer/models/DiscModel.h"
#include "simulation/Planet.h"

class RingedPlanet : public Planet
{
public:
	RingedPlanet();
	RingedPlanet(std::string name, float radius, float mass, float rotationSpeed,
		float revolutionSpeed, float distanceFromSun, float innerRingRadius, float outerRingRadius);
	RingedPlanet(RingedPlanet&& other) noexcept;
	RingedPlanet& operator=(RingedPlanet&& other) noexcept;

	void update(float deltaTime) override;
	void render() const override;

private:
	float ringAngleOffset;

	DiscModel ringModel;
	Texture ringTexture;

	RingedPlanet(const RingedPlanet& other) = delete;
	void operator=(const RingedPlanet& other) = delete;
};
