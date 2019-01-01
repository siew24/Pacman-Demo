#include "Framework.h"
#include <ctime>
#include <Windows.h>

#include <thread>
#include "Header Files/Configs.h"
#include "Header Files/Level.h"
#include "Header Files/PlayerObject.h"
#include "Header Files/Systems/PlayerMovement.h"
#include "getExePath.h"



using namespace bloom;

Game* game = nullptr;

const int WINDOW_WIDTH = 28*TILESIZE;
const int WINDOW_HEIGHT = 31*TILESIZE;

void test_drawer(const std::filesystem::path& assetsPath)
{
	const int fps = 60;
	const int framedelay = (1000 / fps);

	Uint32 framestart;

	game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT, 0 , 6 | SDL_RENDERER_TARGETTEXTURE);
	try {
		game->create("Bloom Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	SDL_Color Background{0,0,0};

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
	PlayerMovement playerMovement(testRegistry);
	Level level_1 = Level(game);
	level_1.load(LevelDir / "0.txt");
	level_1.generateTexture(TileDir);
	level_1.generatePellets(TileDir, testRegistry);
	playerMovement.layout = level_1.layout;

	std::filesystem::path PacDir = assetsPath / L"Pacman.png";
	Player player(testRegistry, game);
	player.init(PacDir);
	auto dt = 0.0;

	while (game->isRunning()) {
		framestart = SDL_GetTicks();
		game->handleEvents();
		
		if (game->input.isKeyPressed(KEY_W)  || game->input.isKeyPressed(KEY_UP))
			testRegistry.get<Pacman>(player.getEntityID()).nextDir = up;
		else if (game->input.isKeyPressed(KEY_A) || game->input.isKeyPressed(KEY_LEFT))
			testRegistry.get<Pacman>(player.getEntityID()).nextDir = left;
		else if (game->input.isKeyPressed(KEY_S) || game->input.isKeyPressed(KEY_DOWN))
			testRegistry.get<Pacman>(player.getEntityID()).nextDir = down;
		else if (game->input.isKeyPressed(KEY_D) || game->input.isKeyPressed(KEY_RIGHT))
			testRegistry.get<Pacman>(player.getEntityID()).nextDir = right;
		else
			testRegistry.get<Pacman>(player.getEntityID()).nextDir = null;

		game->clear();
		level_1.draw();
		animSysTest.update(dt);
		playerMovement.update(dt);
		renderSysTest.update(); // Test again.
		game->render();
		// game->update();

		int frametime = SDL_GetTicks() - framestart;

		if (framedelay > frametime) {
			game->delay(framedelay - frametime);
		}
		dt = game->timer.lap();
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