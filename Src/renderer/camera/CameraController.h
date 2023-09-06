#pragma once

#include <math/Vector.h>
#include "devices/Mouse.h"
#include "devices/Cursor.h"

class Camera;

class CameraController : IMouseScrollListener, ICursorMovementListener
{
public:
	CameraController();
	CameraController(Camera* camera, float movementSpeed, float mouseSensitivity, float cursorSensitivity);
	CameraController(CameraController&& other) noexcept;

	CameraController& operator=(CameraController&& other) noexcept;

	void update();
	void onMouseScroll(float offset);
	void onCursorMove(int deltaX, int deltaY);

private:
	Camera* camera;
	float movementSpeed, scrollSensitivity, cursorSensitivity;

	const float decay = 0.95f;
	float dFOV;

	CameraController(const CameraController& other) = delete;
	void operator=(const CameraController& other) = delete;
};
