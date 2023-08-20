#pragma once

#include <devices/Keyboard.h>
#include <devices/Cursor.h>
#include <devices/Mouse.h>
#include <math/Matrix.h>
#include <math/Vector.h>

#include <renderer/camera/CameraController.h>

class Camera
{
public:
	static const Camera& getActiveCamera();

	Camera();

	Vector getPosition() const;
	float getYaw() const;
	float getPitch() const;
	float getFOV() const;

	void setPosition(Vector newPosition);
	void setYaw(float newYaw);
	void setPitch(float newPitch);
	void setYawAndPitch(float newYaw, float newPitch);
	void setFOV(float newFOV);

	Vector getDirectionVector() const;
	Vector getFrontVector() const;
	Vector getRightVector() const;

	void update();

	Matrix getViewMatrix() const;
	Matrix getProjectionMatrix() const;

	void bind() const;

	CameraController controller;
private:
	static const Camera* activeCamera;

	Vector position;
	float yaw;
	float pitch;
	float fov;

	Vector frontVector, rightVector, upVector;

	void updateViewVectors();

	Camera(const Camera& other) = delete;
	Camera(Camera&& other) = delete;
	void operator=(const Camera& other) = delete;
	void operator=(Camera&& other) = delete;
};

