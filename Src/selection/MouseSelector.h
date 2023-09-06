#pragma once

#include <vector>

#include "devices/Mouse.h"
#include "math/Vector.h"
#include "simulation/SolarSystem.h"
#include "selection/ISelectable.h"

class MouseSelector : public IMouseClickListener
{
public:
	MouseSelector(SolarSystem& solarSystem);

	virtual ~MouseSelector() = default;

	void update();

	std::string getSelectedObjectName() const;

private:
	Vector currentRay;
	SolarSystem& solarSystem;
	ISelectable* selectedObject;

	Vector toEyeCoords(Vector clipCoords);
	Vector toWorldCoords(Vector eyeCoords);

	bool isObjectSelected(const ISelectable* asteroid) const;

	virtual void onMouseClick(int code) override;
	void onLeftClick();
	void onRightClick();
};
