#pragma once

#include "renderer/gui/SpeedButton.h"
#include "renderer/gui/Text.h"
#include "selection/MouseSelector.h"

class GUI: public IMouseClickListener
{
public:
	GUI(const MouseSelector& mouseSelector);

	virtual ~GUI() = default;

	void render() const;
private:
	const MouseSelector& mouseSelector;
	Text helpAddPlanet, helpAddAsteroid;
	SpeedButton speedButtons[4];

	void onMouseClick(int code) override;

	GUI(const GUI& other) = delete;
	GUI(GUI&& other) = delete;
	void operator=(const GUI& other) = delete;
	void operator=(GUI&& other) = delete;
};

