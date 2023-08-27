#pragma once

#include <devices/Window.h> // this includes glew too
#include <iostream>
#include <math/Math.h>

#include "renderer/camera/Camera.h"
#include "renderer/camera/CameraController.h"

#include "utils/Utils.h"

const Camera& Camera::getActiveCamera()
{
	if (activeCamera == nullptr)
	{
		Utils::CrashWithLog("No camera bound!");
	}

	return *activeCamera;
}

Camera::Camera():
	position({0, 128, 0, 0}),
	yaw(180), pitch(-45),
	fov(90.0f), // BACK TO 80.0F
	controller(CameraController(this, 0.01f, 0.5f, 0.1f))
{
	update();
}

Vector Camera::getPosition() const
{
	return position;
}

float Camera::getYaw() const
{
	return yaw;
}

float Camera::getPitch() const
{
	return pitch;
}

float Camera::getFOV() const
{
	return fov;
}

void Camera::setPosition(Vector newPosition)
{
	position = newPosition;
}

void Camera::setYaw(float newYaw)
{
	yaw = newYaw;
	updateViewVectors();
}

void Camera::setPitch(float newPitch)
{
	pitch = Math::Clamp(newPitch, -89, 89);
	updateViewVectors();
}

void Camera::setYawAndPitch(float newYaw, float newPitch)
{
	yaw = newYaw;
	pitch = Math::Clamp(newPitch, -89, 89);
	updateViewVectors();
}

void Camera::setFOV(float newFOV)
{
	fov = Math::Clamp(newFOV, 10.0f, 120.0f);
}

Vector Camera::getDirectionVector() const
{
	return { Math::Cos(yaw), 0, Math::Sin(yaw), 0 };
}

Vector Camera::getFrontVector() const
{
	return this->frontVector;
}

Vector Camera::getRightVector() const
{
	return this->rightVector;
}

void Camera::update()
{
	controller.update();
	updateViewVectors();
	/*
	std::cout << "Camera@" << position.x
		<< "/" << position.y
		<< "/" << position.z
		<< std::endl;*/
}

Matrix Camera::getViewMatrix() const
{
	return {
		rightVector.x, rightVector.y, rightVector.z, - Vector::DotProduct(rightVector, position),
		upVector.x, upVector.y, upVector.z, -Vector::DotProduct(upVector, position),
		-frontVector.x, -frontVector.y, -frontVector.z, Vector::DotProduct(frontVector, position),
		0, 0, 0, 1 };
}

Matrix Camera::getProjectionMatrix() const
{
	const float far = 10000.0f, near = 0.1f;

	Matrix matrix;

	float fov = Math::ToRadians(this->fov);
	float aspectRatio = (float) Window::WIDTH / Window::HEIGHT;
	float tanHalfFov = std::tan(fov / 2);

	matrix.data[0][0] = 1 / (aspectRatio * tanHalfFov);
	matrix.data[1][1] = 1 / tanHalfFov;
	matrix.data[2][2] = -(far + near) / (far - near);
	matrix.data[2][3] = -2 * far * near / (far - near);
	matrix.data[3][2] = -1;

	return matrix;
}

void Camera::bind() const
{
	activeCamera = this;
}

const Camera* Camera::activeCamera = nullptr;

void Camera::updateViewVectors()
{
	// Updating front vector
	frontVector.x = Math::Cos(yaw) * Math::Cos(pitch);
	frontVector.y = Math::Sin(pitch);
	frontVector.z = Math::Sin(yaw) * Math::Cos(pitch);
	frontVector.normalize();

	// Updating right vector
	rightVector = Vector::CrossProduct(frontVector, Vector::up());
	rightVector.normalize();

	// Updating upVector
	// no need to normalize - both unit & perpendicular vectors
	upVector = Vector::CrossProduct(rightVector, frontVector);
}
