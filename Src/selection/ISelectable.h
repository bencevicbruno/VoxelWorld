#pragma once

#include <string>

#include "math/Vector.h"

class ISelectable
{
public:
	virtual ~ISelectable() = default;

	virtual void select() = 0;
	virtual void deselect() = 0;
	virtual bool isSelected() const = 0;

	virtual Vector getPosition() const = 0;
	virtual float getRadius() const = 0;
	virtual std::string getName() const = 0;
};
