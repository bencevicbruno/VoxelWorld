#include "devices/Mouse.h"

Mouse& Mouse::GetInstance()
{
	static Mouse instance;

	return instance;
}

void Mouse::init(GLFWwindow* pWindow)
{
	glfwSetScrollCallback(pWindow,
		[](GLFWwindow* window, double xOffset, double yOffset) -> void
		{
			Mouse::GetInstance().onScroll((float)yOffset);
		});

	glfwSetMouseButtonCallback(pWindow,
		[](GLFWwindow* window, int button, int action, int mods) -> void
		{
			bool pressed = action == GLFW_PRESS;

			if (button == GLFW_MOUSE_BUTTON_LEFT) Mouse::GetInstance().setLeftButton(pressed);
			else if (button == GLFW_MOUSE_BUTTON_MIDDLE) Mouse::GetInstance().setMiddleButton(pressed);
			else if (button == GLFW_MOUSE_BUTTON_RIGHT) Mouse::GetInstance().setRightButton(pressed);

			if (pressed)
			{
				Mouse::GetInstance().onClick(button);
			}
		});
}

bool Mouse::isLeftButtonDown()
{
	return leftButtonDown;
}

bool Mouse::isMiddleButtonDown()
{
	return middleButtonDown;
}

bool Mouse::isRightButtonDown()
{
	return rightButtonDown;
}

void Mouse::subscribeToMouseClick(IMouseClickListener* listener)
{
	Mouse::GetInstance().clickListeners.push_back(listener);
}

void Mouse::subscribeToMouseScroll(IMouseScrollListener* listener)
{
	Mouse::GetInstance().scrollListeners.push_back(listener);
}

void Mouse::unsubscribeFromMouseClick(IMouseClickListener* listener)
{
	auto& listeners = Mouse::GetInstance().clickListeners;

	listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
		[&](const IMouseClickListener* currentListener) -> bool
		{
			return currentListener == listener;
		}
	), listeners.end());
}

void Mouse::unsubscribeFromMouseScroll(IMouseScrollListener* listener)
{
	auto& listeners = Mouse::GetInstance().scrollListeners;;

	listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
		[&](const IMouseScrollListener* currentListener) -> bool
		{
			return currentListener == listener;
		}
	), listeners.end());
}

Mouse::Mouse() :
	leftButtonDown(false),
	middleButtonDown(false),
	rightButtonDown(false)
{
}

void Mouse::setLeftButton(bool isDown)
{
	leftButtonDown = isDown;
}

void Mouse::setMiddleButton(bool isDown)
{
	middleButtonDown = isDown;
}

void Mouse::setRightButton(bool isDown)
{
	rightButtonDown = isDown;
}

void Mouse::onClick(int code)
{
	for (auto listener : clickListeners)
	{
		listener->onMouseClick(code);
	}
}

void Mouse::onScroll(float offset)
{
	for (auto listener : scrollListeners)
	{
		listener->onMouseScroll(offset);
	}
}