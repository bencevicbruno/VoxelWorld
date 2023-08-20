#pragma once

#include "opengl.h"

#include <vector>

class Keyboard
{
public:
	static Keyboard& GetInstance();

	void init(GLFWwindow* pWindow);

	static bool isKeyDown(int keyCode);
	
private:
	bool keyState[GLFW_KEY_LAST];

	Keyboard();

	Keyboard(const Keyboard& other) = delete;
	Keyboard(Keyboard&& other) = delete;
	void operator=(const Keyboard& other) = delete;
	void operator=(Keyboard&& other) = delete;
};

