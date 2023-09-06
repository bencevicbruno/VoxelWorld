#pragma once

#include <string>

#include <renderer/wrappers/Texture.h>
#include "renderer/models/SphereModel.h"
#include "selection/ISelectable.h"

class Planet : public ISelectable
{
public:
	Planet();
	Planet(std::string name, float radius, float mass, float rotationSpeed, float revolutionSpeed, float distanceFromSun);
	Planet(std::string name, float radius, float mass, float rotationSpeed, float revolutionSpeed, float distanceFromSun,
		float rotationAngle, float revolutionAngle, float revolutionPitch, Texture texture, SphereModel model);
	Planet(Planet&& other) noexcept;
	Planet& operator=(Planet&& other) noexcept;

	virtual void update(float deltaTime);
	virtual void render() const;

	float getMass() const;

	/* ISelectable */
	void select() override;
	void deselect() override;
	bool isSelected() const override;

	Vector getPosition() const override;
	float getRadius() const override;
	std::string getName() const override;

	virtual ~Planet() = default;

	static Planet* CreateRandom();
protected:
	std::string name;
	float radius;
	float mass;
	float rotationAngle;
	float rotationSpeed;
	float revolutionAngle;
	float revolutionSpeed;
	float distanceFromSun;
	float revolutionPitch;

	SphereModel model;
	Texture texture;

	bool selected;

	Planet(const Planet& other) = delete;
	void operator=(const Planet& other) = delete;
};