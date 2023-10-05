#include <cstdlib>
#include <ctime>
#include <chrono>

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
	//performPerlinNoiseTests();
	//return 0;

	std::thread::id threadID = std::this_thread::get_id();
	std::cout << "Main threadID: " << threadID << std::endl;
	initBlockRegistry();
	
	//performFileManagerTests();
	//performImageManagerTests();
	
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
	//Skybox skybox(Color::CreateFromRGB(255, 255, 255));

	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	unsigned int currentTime = static_cast<unsigned int>(time); //1 << 31;//static_cast<unsigned int>(time);
	std::cout << "Seed: " << currentTime << std::endl;

	World world(currentTime, CHUNKS_COUNT_TO_GENERATE);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	auto last_time = std::chrono::high_resolution_clock::now();

	if (RENDER_MESH_FANS)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	while (!window.shouldClose())
	{
		window.update();
		camera.update();
		world.update(camera.getPosition());

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.render();

		last_time = std::chrono::high_resolution_clock::now();
		world.render();
		//auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - last_time).count();
		//std::cout << duration << std::endl;

		//auto fps = duration == 0 ? "X fps" : (std::to_string(1000 / duration) + " fps");
		//window.setTitle("Voxel World " + fps);
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
	registry.registerBlock(BLOCK_WATER_SURFACE, BlockMesh(UVPosition::FromAtlas(4, 0), Box(-0.5f, -0.5f, -0.5f, 0.5f, 0.5f - 2.0f / 16.0f, 0.5f)), BlockOpacity::TRANSPARENT);
	registry.registerBlock(BLOCK_SNOW, BlockMesh(UVPosition::FromAtlas(5, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_OAK_LOG, BlockMesh(UVPosition::FromAtlas(8, 0), UVPosition::FromAtlas(7, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_OAK_LEAVES, BlockMesh(UVPosition::FromAtlas(9, 0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_TALL_GRASS, BlockMesh(UVPosition::FromAtlas(10, 0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_GRAVEL, BlockMesh(UVPosition::FromAtlas(11, 0)), BlockOpacity::OPAQUE);
	registry.registerBlock(BLOCK_FLOWER_YELLOW, BlockMesh(UVPosition::FromAtlas(12, 0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_FLOWER_RED, BlockMesh(UVPosition::FromAtlas(13, 0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_PODZOL, BlockMesh(UVPosition::FromAtlas(15, 0), UVPosition::FromAtlas(1, 0), UVPosition::FromAtlas(14, 0)), BlockOpacity::OPAQUE);

	registry.registerBlock(BLOCK_SEDGE, BlockMesh(UVPosition::FromAtlas(0, 2)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_SEDGE_TOP, BlockMesh(UVPosition::FromAtlas(0, 1)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_LILY_PAD, BlockMesh(UVPosition::FromAtlas(2, 1), Box(-0.5, 0.5, 1.0 / 256.0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_LILY_PADS, BlockMesh(UVPosition::FromAtlas(3, 1), Box(-0.5, 0.5, 1.0 / 256.0)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_HUGE_GRASS, BlockMesh(UVPosition::FromAtlas(1, 2)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_HUGE_GRASS_TOP, BlockMesh(UVPosition::FromAtlas(1, 1)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_MUSHROOM_BROWN, BlockMesh(UVPosition::FromAtlas(4, 1)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_MUSHROOM_RED, BlockMesh(UVPosition::FromAtlas(5, 1)), BlockOpacity::SEE_THROUGH);
	registry.registerBlock(BLOCK_DEAD_LEAVES, BlockMesh(UVPosition::FromAtlas(6, 1), Box(-0.5, 0.5, 1.0 / 256.0)), BlockOpacity::SEE_THROUGH);

	registry.registerBlock(BLOCK_SPEACIAL_FLOWER, BlockMesh(UVPosition::FromAtlas(7, 1)), BlockOpacity::SEE_THROUGH);
}