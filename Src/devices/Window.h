#pragma once

#include "opengl.h"

#include <string>

#include "math/Matrix.h"

class Window
{
public:
	static const int WIDTH = 1600, HEIGHT = 900;

	static Window& GetInstance();

	std::string getTitle() const;
	void setTitle(const std::string& newTitle);
	bool isRunning() const;
	bool hasCursor() const;

	void update();
	void close();
	bool shouldClose() const;

	void grabCursor();
	void releaseCursor();

	void setSize(int width, int height);

	Matrix getOrthogonalMatrix() const;

private:
	std::string title;
	bool running;
	bool isCursorGrabbed;
	int width, height;
	GLFWwindow* pWindow;

	Window(std::string title);

	~Window();

	void initGLFW() const;
	void initGLEW() const;

	Window(const Window& other) = delete;
	Window(Window&& other) = delete;
	void operator=(const Window& other) = delete;
	void operator=(Window&& other) = delete;
};

