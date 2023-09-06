#include "selection/MouseSelector.h"

#include "devices/Cursor.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include "renderer/camera/Camera.h"
#include "simulation/Asteroid.h"
#include "simulation/Planet.h"

MouseSelector::MouseSelector(SolarSystem& solarSystem) :
	currentRay({}),
	solarSystem(solarSystem),
	selectedObject(nullptr)
{}

void MouseSelector::update()
{
	Vector clipCoords = Vector(Cursor::getNormalizedX(), Cursor::getNormalizedY(), -1.0f, 1.0f);
	Vector eyeCoords = toEyeCoords(clipCoords);
	Vector worldCoords = toWorldCoords(eyeCoords);

	currentRay = worldCoords;
}

Vector MouseSelector::toEyeCoords(Vector clipCoords)
{
	Matrix invertedProjection = Camera::getActiveCamera().getProjectionMatrix();
	invertedProjection.invert();
	Vector eyeCoords = invertedProjection * clipCoords;

	return Vector(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

Vector MouseSelector::toWorldCoords(Vector eyeCoords)
{
	Matrix invertedView = Camera::getActiveCamera().getViewMatrix();
	invertedView.invert();
	Vector worldCoords = invertedView * eyeCoords;
	worldCoords.normalize();

	return worldCoords;
}

std::string MouseSelector::getSelectedObjectName() const
{
	return selectedObject ? selectedObject->getName() : "";
}

bool MouseSelector::isObjectSelected(const ISelectable* object) const
{
	if (object == nullptr) return false;

	Vector center = object->getPosition();
	float radius = object->getRadius();
	Vector point = Camera::getActiveCamera().getPosition();
	Vector unit = this->currentRay;

	Vector q = point - center;
	float a = Vector::DotProduct(unit, unit);
	float b = 2 * Vector::DotProduct(unit, q);
	float c = Vector::DotProduct(q, q) - radius * radius;
	float d = b * b - 4 * a * c;

	return d >= 0;
}

void MouseSelector::onMouseClick(int code)
{
	if (code == GLFW_MOUSE_BUTTON_LEFT)
	{
		onLeftClick();
	}

	else if (code == GLFW_MOUSE_BUTTON_RIGHT)
	{
		onRightClick();
	}
}

void MouseSelector::onLeftClick()
{
	if (selectedObject != nullptr)
	{
		selectedObject->deselect();
		selectedObject = nullptr;
	}

	for (const auto& planet : solarSystem.getPlanets())
	{
		if (isObjectSelected(planet))
		{
			planet->select();
			selectedObject = planet;
			return;
		}
	}

	for (const auto& asteroid : solarSystem.getAsteroids())
	{
		if (isObjectSelected(asteroid))
		{
			asteroid->select();
			selectedObject = asteroid;
			return;
		}
	}
}

void MouseSelector::onRightClick()
{
	if (isObjectSelected(selectedObject))
	{
		if (Planet* planet = dynamic_cast<Planet*>(selectedObject))
		{
			solarSystem.removePlanet(planet);
		}
		else if (Asteroid* asteroid = dynamic_cast<Asteroid*>(selectedObject))
		{
			solarSystem.removeAsteroid(asteroid);
		}

		selectedObject = nullptr;
	}
}