#pragma once

#include <renderer/wrappers/Texture.h>
#include "renderer/models/SphereModel.h"
#include "math/Vector.h"
#include "simulation/SolarSystem.h"
#include "selection/ISelectable.h"

class Asteroid : public ISelectable
{
public:
	Asteroid();
	Asteroid(Asteroid&& other) noexcept;
	Asteroid& operator=(Asteroid&& other) noexcept;

	void update(float deltaTime, const Vector& gravitationalForce);
	void render() const;

	/* ISelectable */
	void select() override;
	void deselect() override;
	bool isSelected() const override;

	Vector getPosition() const override;
	float getRadius() const override;
	std::string getName() const override;

	virtual ~Asteroid() = default;

private:
	std::string name;
	float radius;
	Vector position, velocity, acceleration;
	Texture texture;
	SphereModel model;
	bool selected;

	Asteroid(const Asteroid& other) = delete;
	void operator=(const Asteroid& other) = delete;
};