#include <renderer/camera/CameraController.h>

#include <math/Math.h>
#include <renderer/camera/Camera.h>
#include "devices/Keyboard.h"

CameraController::CameraController() :
	camera(nullptr),
	movementSpeed(0),
	scrollSensitivity(0),
	cursorSensitivity(0),
	dFOV(0.0f)
{}

CameraController::CameraController(Camera* camera, float movementSpeed, float mouseSensitivity, float cursorSensitivity) :
	camera(camera),
	movementSpeed(movementSpeed),
	scrollSensitivity(mouseSensitivity),
	cursorSensitivity(cursorSensitivity),
	dFOV(0.0f)
{}

CameraController::CameraController(CameraController&& other) noexcept
{
	*this = std::move(other);
}

CameraController& CameraController::operator=(CameraController&& other) noexcept
{
	if (this == &other) return *this;

	camera = std::exchange(other.camera, nullptr);
	movementSpeed = std::exchange(other.movementSpeed, 0.0f);
	scrollSensitivity = std::exchange(other.scrollSensitivity, 0.0f);
	cursorSensitivity = std::exchange(other.scrollSensitivity, 0.0f);
	dFOV = std::exchange(other.dFOV, 0.0f);

	return *this;
}

void CameraController::update()
{
	camera->setFOV(camera->getFOV() + dFOV);
	dFOV *= decay;

	Vector dPosition;

	if (Keyboard::isKeyDown(GLFW_KEY_SPACE))
	{
		dPosition += Vector::up();
	}

	if (Keyboard::isKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		dPosition += Vector::down();
	}

	if (Keyboard::isKeyDown(GLFW_KEY_W))
	{
		dPosition += camera->getDirectionVector();
	}

	if (Keyboard::isKeyDown(GLFW_KEY_S))
	{
		dPosition -= camera->getDirectionVector();
	}

	if (Keyboard::isKeyDown(GLFW_KEY_A))
	{
		dPosition -= camera->getRightVector();
	}

	if (Keyboard::isKeyDown(GLFW_KEY_D))
	{
		dPosition += camera->getRightVector();
	}

	dPosition *= 0.5;

	camera->setPosition(camera->getPosition() + dPosition);
}

void CameraController::onMouseScroll(float offset)
{
	dFOV = -offset*scrollSensitivity;
}

void CameraController::onCursorMove(int deltaX, int deltaY)
{
	camera->setYawAndPitch(
		camera->getYaw() - deltaX * cursorSensitivity,
		camera->getPitch() + deltaY * cursorSensitivity);
}