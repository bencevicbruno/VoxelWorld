#pragma once

#include <vector>
#include <chrono>

#include "renderer/wrappers/VertexArray.h"
#include "devices/Keyboard.h"
#include "simulation/Planet.h"
#include "simulation/Sun.h"

class Asteroid;

class SolarSystem
{
public:
	SolarSystem();

	const Sun& getSun() const;
	const std::vector<Planet*>& getPlanets() const;
	const std::vector<Asteroid*>& getAsteroids() const;

	void update();
	void render() const;

	void addPlanet(Planet* planet);
	void removePlanet(const Planet* planet);

	void addAsteroid(Asteroid* asteroid);
	void removeAsteroid(const Asteroid* asteroid);

	bool hasColided(Asteroid* asteroid);
	Vector getGravitationalForceFor(Asteroid* asteroid);

	~SolarSystem();
private:
	std::chrono::system_clock::time_point lastTime;
	const Sun sun;
	std::vector<Planet*> planets;
	std::vector<Asteroid*> asteroids;

	void processKeyboard();

	SolarSystem(const SolarSystem& other) = delete;
	SolarSystem(SolarSystem&& other) = delete;
	void operator=(const SolarSystem& other) = delete;
	void operator=(SolarSystem&& other) = delete;
};