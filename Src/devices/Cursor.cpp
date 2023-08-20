#include "devices/Cursor.h"

#include <iostream>

#include "devices/Window.h"

Cursor& Cursor::GetInstance()
{
	static Cursor instance;

	return instance;
}

void Cursor::init(GLFWwindow* pWindow)
{
	glfwSetCursorPosCallback(pWindow,
		[](GLFWwindow* window, double xPos, double yPos) -> void
		{
			Cursor::GetInstance().onMove((int)xPos, (int)yPos);
		});
}

int Cursor::getX()
{
	return Cursor::GetInstance().xPos;
}

int Cursor::getY()
{
	return Cursor::GetInstance().yPos;
}

float Cursor::getNormalizedX()
{
	return 2.0f * Cursor::getX() / Window::WIDTH - 1.0f;
}

float Cursor::getNormalizedY()
{
	return -(2.0f * Cursor::getY() / Window::HEIGHT - 1.0f);
}

void Cursor::subscribeToCursorMovement(ICursorMovementListener* listener)
{
	Cursor::GetInstance().movementListeners.push_back(listener);
}

void Cursor::unsubscribeToCursorMovement(ICursorMovementListener* listener)
{
	auto& listeners = Cursor::GetInstance().movementListeners;

	listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
		[&](const ICursorMovementListener* currentListener) -> bool
		{
			return currentListener == listener;
		}
	), listeners.end());
}

Cursor::Cursor() :
	xPos(0), yPos(0)
{
}

void Cursor::onMove(int x, int y)
{
	if (!Window::GetInstance().hasCursor()) return;

	if (xPos != x || yPos != y)
	{
		for (auto listener : movementListeners)
		{
			listener->onCursorMove(xPos - x, yPos - y);
		}

		xPos = x;
		yPos = y;
	}
}