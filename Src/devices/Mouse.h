#pragma once

#include "opengl.h"

#include <vector>

class IMouseClickListener;
class IMouseScrollListener;

class Mouse
{
public:
	static Mouse& GetInstance();

	void init(GLFWwindow* pWindow);

	bool isLeftButtonDown();
	bool isMiddleButtonDown();
	bool isRightButtonDown();

	static void subscribeToMouseClick(IMouseClickListener* listener);
	static void subscribeToMouseScroll(IMouseScrollListener* listener);

	static void unsubscribeFromMouseClick(IMouseClickListener* listener);
	static void unsubscribeFromMouseScroll(IMouseScrollListener* listener);

private:
	bool leftButtonDown, middleButtonDown, rightButtonDown;
	std::vector<IMouseClickListener*> clickListeners;
	std::vector<IMouseScrollListener*> scrollListeners;

	Mouse();

	void setLeftButton(bool isDown);
	void setMiddleButton(bool isDown);
	void setRightButton(bool isDown);

	void onClick(int code);
	void onScroll(float offset);

	Mouse(const Mouse& other) = delete;
	Mouse(Mouse&& other) = delete;
	void operator=(const Mouse& other) = delete;
	void operator=(Mouse&& other) = delete;
};

class IMouseClickListener
{
public:
	IMouseClickListener()
	{
		Mouse::subscribeToMouseClick(this);
	}

	virtual void onMouseClick(int code) = 0;

	virtual ~IMouseClickListener()
	{
		Mouse::unsubscribeFromMouseClick(this);
	}
};

class IMouseScrollListener
{
public:
	IMouseScrollListener()
	{
		Mouse::subscribeToMouseScroll(this);
	}

	virtual void onMouseScroll(float offset) = 0;

	virtual ~IMouseScrollListener()
	{
		Mouse::unsubscribeFromMouseScroll(this);
	}
};