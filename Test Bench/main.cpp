#include "Framework.h"
#include <ctime>
#include <Windows.h>

#include <thread>

#include "Header Files/AnimatedObject.h"
#include "Header Files/Level.h"
#include "Header Files/MovementSystem.h"
#include "getExePath.h"

using namespace bloom;
using namespace bloom::audio;
using namespace std::chrono_literals;
using bloom::components::Position;
using bloom::components::Size;
using Layout = std::vector<std::vector<std::filesystem::path>>;

Game* game = nullptr;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

const int TILE_WIDTH = 19;
const int TILE_HEIGHT = 19;

void test_drawer(const std::filesystem::path& assetsPath)
{
	const int fps = 60;
	const int framedelay = (1000 / fps);

	Uint32 framestart;

	game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT);
	try {
		game->create("Bloom Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	SDL_Color Background{0,0,0,0};

	game->setColor(Background);
	game->clear();
	game->render();

	if (!std::filesystem::exists(assetsPath))
		throw bloom::Exception("Required assets can't be found.");

	std::filesystem::path TileDir = assetsPath / L"Tile";
	std::filesystem::path LevelDir = assetsPath / L"Level";

	entt::DefaultRegistry testRegistry;
	bloom::systems::RenderSystem renderSysTest(testRegistry);
	bloom::systems::AnimationSystem animSysTest(testRegistry);
	MovementSystem moveSysTest(testRegistry);
	Level Level_1 = Level(assetsPath, TileDir, LevelDir);
	Level_1.initLevel(testRegistry, game);
	Level_1.generate(testRegistry);

	while (game->isRunning()) {
		if (game->input.isKeyDown(KEY_W) || game->input.isKeyPressed(KEY_W))
			testRegistry.replace<Direction>(Level_1.getPacmanID(), KEY_W);
		if (game->input.isKeyDown(KEY_A) || game->input.isKeyPressed(KEY_A))
			testRegistry.replace<Direction>(Level_1.getPacmanID(), KEY_A);
		if (game->input.isKeyDown(KEY_S) || game->input.isKeyPressed(KEY_S))
			testRegistry.replace<Direction>(Level_1.getPacmanID(), KEY_S);
		if (game->input.isKeyDown(KEY_D) || game->input.isKeyPressed(KEY_D))
			testRegistry.replace<Direction>(Level_1.getPacmanID(), KEY_D);
		// Demo ends here.
		framestart = SDL_GetTicks();
		game->handleEvents();
		game->clear();
		moveSysTest.update();
		animSysTest.update(game->timer.lap());
		renderSysTest.update(); // Test again.
		game->render();
		// game->update();

		int frametime = SDL_GetTicks() - framestart;

		if (framedelay > frametime) {
			game->delay(framedelay - frametime);
		}
	}
	game->destroy();
}

int main()
{
	SetConsoleCP(CP_UTF8); SetConsoleOutputCP(CP_UTF8);

	try {
		Game::initialize();
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
		system("pause");
		exit(-1);
	}

	namespace fs = std::filesystem;
	fs::path dataDir = fs::path(getExePath()) / L"data";
	fs::path assetsPath = dataDir / L"Assets";
	fs::path musicPath = dataDir / L"Music";
	fs::path soundsPath = dataDir / L"Sounds";

	std::thread drawer_thread{ test_drawer, assetsPath };

	drawer_thread.join();

	return 0;
}