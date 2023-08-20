#include "devices/Keyboard.h"

#include <algorithm>

#include "devices/Window.h"

Keyboard& Keyboard::GetInstance()
{
	static Keyboard instance;

	return instance;
}

void Keyboard::init(GLFWwindow* pWindow)
{
	glfwSetKeyCallback(pWindow,
		[](GLFWwindow* window, int key, int scancode, int action, int mode) -> void
		{
			Keyboard& keyboard = Keyboard::GetInstance();

			keyboard.keyState[key] = action == GLFW_PRESS || action == GLFW_REPEAT;

			if (key == GLFW_KEY_E && action == GLFW_PRESS)
			{
				Window& window = Window::GetInstance();

				if (window.hasCursor())
				{
					window.releaseCursor();
				}
				else
				{
					window.grabCursor();
				}
			}
		});
}

bool Keyboard::isKeyDown(int keyCode)
{
	return Keyboard::GetInstance().keyState[keyCode];
}

Keyboard::Keyboard()
{
	for (int i = 0; i < GLFW_KEY_LAST; i++)
	{
		keyState[i] = false;
	}
}