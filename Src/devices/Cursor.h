#pragma once

#include "opengl.h"
#include "GLFW/glfw3.h"

#include <vector>

class ICursorMovementListener;

class Cursor
{
public:
	static Cursor& GetInstance();

	void init(GLFWwindow* pWindow);

	static int getX();
	static int getY();

	static float getNormalizedX();
	static float getNormalizedY();

	static void subscribeToCursorMovement(ICursorMovementListener* listener);
	static void unsubscribeToCursorMovement(ICursorMovementListener* listener);

private:
	int xPos, yPos;
	std::vector<ICursorMovementListener*> movementListeners;

	Cursor();

	void onMove(int x, int y);
	
	Cursor(const Cursor& other) = delete;
	Cursor(Cursor&& other) = delete;
	void operator=(const Cursor& other) = delete;
	void operator=(Cursor&& other) = delete;
};

class ICursorMovementListener
{
public:
	ICursorMovementListener()
	{
		Cursor::GetInstance().subscribeToCursorMovement(this);
	}

	virtual void onCursorMove(int deltaX, int deltaY) = 0;

	virtual ~ICursorMovementListener()
	{
		Cursor::GetInstance().unsubscribeToCursorMovement(this);
	}
};