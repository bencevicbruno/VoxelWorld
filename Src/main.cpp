#include <cstdlib>
#include <ctime>

#include "devices/Window.h"
#include "renderer/camera/Camera.h"
#include "renderer/gui/GUI.h"
#include "simulation/Earth.h"
#include "simulation/RingedPlanet.h"
#include "simulation/Skybox.h"
#include "simulation/SolarSystem.h"
#include "selection/MouseSelector.h"
#include "renderer/shader/ShaderProgramRegistry.h"
#include "world/utils/BlockRegistry.h"

#include "renderer/models/BlockModel.h"
#include "math/PerlinNoiseGenerator.h"

#include "world/World.h"
#include "_tests/FileManagerTests.h"
#include "_tests/ImageManagerTests.h"
#include "_tests/PerlinNoiseImagesTest.h"
#include "world/Blocks.h"

#include "renderer/models/BlockMesh.h"

void initBlockRegistry();

int main()
{
	std::thread::id threadID = std::this_thread::get_id();
	std::cout << "Main threadID: " << threadID << std::endl;
	initBlockRegistry();
	/*
	performFileManagerTests();
	//performImageManagerTests();
	performPerlinNoiseTests();
	//return 0;*/
	std::srand((unsigned int)std::time(nullptr));
	Window& window = Window::GetInstance();

	Camera camera;
	camera.bind();

	ImageManager::GetInstance().getImage("atlas.png");

	ShaderProgramRegistry::GetInstance().loadShader("gui");
	ShaderProgramRegistry::GetInstance().loadShader("object");
	ShaderProgramRegistry::GetInstance().loadShader("sun");
	ShaderProgramRegistry::GetInstance().loadShader("earth");
	ShaderProgramRegistry::GetInstance().loadShader("skybox");

	Skybox skybox(Color::CreateFromRGB(173, 216, 230));
	World world(0, camera.getPosition());


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	while (!window.shouldClose())
	{
		window.update();
		camera.update();
		world.update(camera.getPosition());
		//solarSystem.update();
		//mouseSelector.update();

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//solarSystem.render();

		skybox.render();
		world.render();
	}

	return 0;
}

void initBlockRegistry()
{
	BlockRegistry& registry = BlockRegistry::GetInstance();

	registry.registerBlock(BLOCK_STONE, BlockMesh(UVPosition::FromAtlas(0, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_DIRT, BlockMesh(UVPosition::FromAtlas(1, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_GRASS, BlockMesh(UVPosition::FromAtlas(3, 0), UVPosition::FromAtlas(1, 0), UVPosition::FromAtlas(2, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_SAND, BlockMesh(UVPosition::FromAtlas(6, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_WATER, BlockMesh(UVPosition::FromAtlas(4, 0)), BlockOpacity::TRANSPARENT);
	registry.registerBlock(BLOCK_WATER_SURFACE, BlockMesh(UVPosition::FromAtlas(4, 0), Box(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f - 2.0f/16.0f, 0.5f)), BlockOpacity::TRANSPARENT);
	registry.registerBlock(BLOCK_SNOW, BlockMesh(UVPosition::FromAtlas(5, 0)), BlockOpacity::OPAQUE);
}