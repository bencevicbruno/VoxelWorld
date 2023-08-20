#pragma once


#include "renderer/wrappers/VertexArray.h"
#include "renderer/wrappers/Buffer.h"
#include <renderer/wrappers/Texture.h>

class SpeedButton
{
public:
	SpeedButton();
	SpeedButton(const std::string& name, float speedFactor, int x, int y);
	SpeedButton(SpeedButton&& other) noexcept;

	SpeedButton& operator=(SpeedButton&& other) noexcept;

	void select();
	void deselect();

	void render() const;
	void onClick();

	bool isHoveredOn() const;

	static float simulationSpeedFactor;
	static float getSimulationSpeedFactor();

	static const int SIZE = 64;

private:
	int x, y;
	
	Texture texture, textureHovered, textureSelected;
	float speedFactor;
	bool selected;

	VertexArray vertexArray;
	Buffer vertexBuffer, indexBuffer;

	void setupVertexBuffer();
	void setupIndexBuffer();

	SpeedButton(const SpeedButton& other) = delete;
	SpeedButton& operator=(const SpeedButton& other) = delete;
};

