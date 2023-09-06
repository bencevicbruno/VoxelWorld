#include <simulation/SolarSystem.h>

#include "simulation/Asteroid.h"
#include "simulation/Earth.h"

#include "renderer/gui/SpeedButton.h"

SolarSystem::SolarSystem() :
	lastTime(std::chrono::system_clock::now()),
	sun(Sun())
{}

const Sun& SolarSystem::getSun() const
{
	return sun;
}

const std::vector<Planet*>& SolarSystem::getPlanets() const
{
	return planets;
}

const std::vector<Asteroid*>& SolarSystem::getAsteroids() const
{
	return asteroids;
}

void SolarSystem::update()
{
	float deltaTime = (std::chrono::system_clock::now() - lastTime).count() * SpeedButton::getSimulationSpeedFactor();

	processKeyboard();

	for (auto& planet : planets)
		planet->update(deltaTime);

	std::vector<Asteroid*> asteroidsToRemove;

	for (auto& asteroid : asteroids)
	{
		if (this->hasColided(asteroid))
		{
			asteroidsToRemove.push_back(asteroid);
			continue;
		}

		Vector gravitationalForce = this->getGravitationalForceFor(asteroid);
		asteroid->update(deltaTime, gravitationalForce);
	}

	for (auto& asteroid : asteroidsToRemove)
		removeAsteroid(asteroid);

	lastTime = std::chrono::system_clock::now();
}

void SolarSystem::render() const
{
	glEnable(GL_DEPTH_TEST);
	sun.render();

	for (const auto& planet : planets)
		planet->render();

	for (const auto& asteroid : asteroids)
		asteroid->render();
}

void SolarSystem::addPlanet(Planet* planet)
{
	planets.push_back(planet);
}

void SolarSystem::removePlanet(const Planet* planet)
{
	planets.erase(std::remove_if(planets.begin(), planets.end(),
		[&](const Planet* currentPlanet) -> bool
		{
			return currentPlanet == planet;
		}
	), planets.end());

	delete planet;
}

void SolarSystem::addAsteroid(Asteroid* asteroid)
{
	asteroids.push_back(asteroid);
}

void SolarSystem::removeAsteroid(const Asteroid* asteroid)
{
	asteroids.erase(std::remove_if(asteroids.begin(), asteroids.end(),
		[&](const Asteroid* currentAsteroid) -> bool
		{
			return currentAsteroid == asteroid;
		}
	), asteroids.end());

	delete asteroid;
}

bool SolarSystem::hasColided(Asteroid* asteroid)
{
	float distanceSquared = asteroid->getPosition().length2();
	float sunRadiusSquared = this->sun.getRadius();
	sunRadiusSquared *= sunRadiusSquared;

	if (distanceSquared - sunRadiusSquared < Math::EPSILON) return true;

	for (const auto planet : this->planets)
	{
		float distanceSquared = (planet->getPosition() - asteroid->getPosition()).length2();
		float minimalDistanceSquared = planet->getRadius() + asteroid->getRadius();
		minimalDistanceSquared *= minimalDistanceSquared;

		if (distanceSquared - minimalDistanceSquared < Math::EPSILON) return true;
	}

	return false;
}

Vector SolarSystem::getGravitationalForceFor(Asteroid* asteroid)
{
	const float GRAVITATIONAL_CONSTANT = 0.001f;

	Vector totalForce;

	for (const auto planet : this->planets)
	{
		Vector direction = planet->getPosition() - asteroid->getPosition();
		float distanceSquared = direction.length2();
		direction.normalize();

		if (distanceSquared < Math::EPSILON) continue;

		Vector force = direction * planet->getMass() / distanceSquared;
		totalForce += force;
	}

	return totalForce * GRAVITATIONAL_CONSTANT;
}

SolarSystem::~SolarSystem()
{
	for (auto planet : planets) delete planet;
	for (auto asteroid : asteroids) delete asteroid;
}

void SolarSystem::processKeyboard()
{
	const Keyboard& keyboard = Keyboard::GetInstance();

	if (keyboard.isKeyDown(GLFW_KEY_I)) {
		addPlanet(Planet::CreateRandom());
	}
	else if (keyboard.isKeyDown(GLFW_KEY_O)) {
		addAsteroid(new Asteroid());
	}
}