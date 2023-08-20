#include "renderer/gui/GUI.h"

#include "devices/Window.h"
#include "devices/Cursor.h"

GUI::GUI(const MouseSelector& mouseSelector) :
	mouseSelector(mouseSelector),
	helpAddPlanet(Text("Press 'I' to add a planet", 1, 650)),
	helpAddAsteroid(Text("Press 'O' to add an asteroid", 1, 700))
{
	const int WIDTH = Window::WIDTH;
	const int HEIGHT = Window::HEIGHT - SpeedButton::SIZE;

	std::string names[] = {"pause", "speed_1x", "speed_2x", "speed_3x"};
	float speeds[] = { 0.0f, 10e-6f, 10e-5f, 10e-4f };
	int xPos[] = { 4, 3, 2, 1 };

	for (int i = 0; i < 4; i++)
	{
		speedButtons[i] = std::move(SpeedButton(names[i], 
			speeds[i], WIDTH - xPos[i] * SpeedButton::SIZE, HEIGHT));
	}
}

void GUI::render() const
{
	helpAddPlanet.render();
	helpAddAsteroid.render();

	Text selectedObject(mouseSelector.getSelectedObjectName(), 0, 0);
	selectedObject.centerHorizontally();
	selectedObject.render();

	for (int i = 0; i < 4; i++)
		speedButtons[i].render();
}

void GUI::onMouseClick(int code)
{
	if (code != GLFW_MOUSE_BUTTON_LEFT) return;

	const Cursor& cursor = Cursor::GetInstance();

	int x = cursor.getX();
	int y = cursor.getY();

	bool cursorOverButtons =
		x >= Window::WIDTH - 4 * SpeedButton::SIZE
		&& y >= Window::HEIGHT - SpeedButton::SIZE;

	if (cursorOverButtons)
	{
		for (int i = 0; i < 4; i++)
		{
			auto& button = speedButtons[i];

			if (button.isHoveredOn())
			{
				button.select();
				button.onClick();
			}
			else
			{
				button.deselect();
			}
		}
	}
}