#include "Framework.h"
#include <ctime>
#include <Windows.h>

#include <thread>
#include "inc/Configs.h"
#include "inc/Level.h"
#include "inc/PlayerObject.h"
#include "inc/Systems/PlayerMovement.h"
#include "inc/Systems/PelletSystem.h"
#include "inc/GhostObject.h"
#include "inc/Systems/GhostAI.h"
#include "getExePath.h"

using namespace bloom;
using namespace bloom::audio;

Game* game = nullptr;

const int WINDOW_WIDTH = 28 * TILESIZE;
const int WINDOW_HEIGHT = 31 * TILESIZE;

void test_drawer(const std::filesystem::path& assetsPath)
{
	const int fps = 60;
	const int framedelay = (1000 / fps);

	Uint32 framestart;

	game = new Game(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 6 | SDL_RENDERER_TARGETTEXTURE);
	try {
		game->create("Bloom Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	SDL_Color Background{ 0,0,0 };

	game->setColor(Background);
	game->clear();
	game->render();

	if (!std::filesystem::exists(assetsPath))
		throw bloom::Exception("Required assets can't be found.");

	std::filesystem::path tileDir = assetsPath / L"Tile";
	std::filesystem::path levelDir = assetsPath / L"Level";
	std::filesystem::path audioDir = assetsPath.parent_path() / L"Sounds";

	entt::DefaultRegistry testRegistry;
	bloom::systems::RenderSystem renderSysTest(testRegistry);
	bloom::systems::AnimationSystem animSysTest(testRegistry);
	PlayerMovement playerMovement(testRegistry);
	GhostAI ghostMovement(testRegistry);
	PelletSystem pelletSystem(testRegistry);
	Level level = Level(game);
	level.changeLevel(levelDir / "0.txt", tileDir, testRegistry);
	playerMovement.layout = level.layout;
	ghostMovement.layout = level.layout;

	std::filesystem::path pacDir = assetsPath / L"Pacman.png";
	std::filesystem::path ghostDir = assetsPath;
	Player player(testRegistry, game);
	player.init(pacDir);

	GhostObject shadow(testRegistry, game); 
	shadow.init(ghostDir, Ghosts::shadow, Tile{13,11});

	GhostObject speedy(testRegistry, game);
	speedy.init(ghostDir, Ghosts::speedy, Tile{ 14,11 });

	GhostObject bashful(testRegistry, game);
	bashful.init(ghostDir, Ghosts::bashful, Tile{ 15,11 });

	GhostObject pokey(testRegistry, game);
	pokey.init(ghostDir, Ghosts::pokey, Tile{ 12,11 });
	
	level.draw();
	animSysTest.update(0);
	renderSysTest.update();
	game->render();
	sounds.add(audioDir / "pacman_beginning.wav");
	sounds.add(audioDir / "pacman_intermission.wav");
	sounds.add(audioDir / "pacman_death.wav");
	sounds[0]->play();
	SDL_Delay(5000);

	auto dt = 0.0;
	int frameCount = 0;
	std::cout << "Level is started!" << std::endl;
	while (game->isRunning()) {
		if (!frameCount)
			std::cout << "Current score:  " << testRegistry.get<Pacman>(player.getEntityID()).score << std::endl;

		std::cout << "Delta time: " << dt << "ms" << std::endl;
		frameCount = (frameCount + 1) % 60;
		game->handleEvents();

		if (game->input.isKeyPressed(KEY_W) || game->input.isKeyPressed(KEY_UP))
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
		level.draw();
		playerMovement.update(dt);
		ghostMovement.update(dt);
		pelletSystem.update();
		animSysTest.update(dt);
		renderSysTest.update(); // Test again.
		game->render();
		// game->update();
		dt = game->timer.lap();

		if (testRegistry.get<Pacman>(player.getEntityID()).pelletsEaten == level.pelletCount()) {
			std::cout << "Level complete!" << std::endl;
			sounds[1]->play();
			game->delay(5500);
			level.changeLevel(levelDir / "0.txt", tileDir, testRegistry);
			player.init(pacDir);
			shadow.init(ghostDir, Ghosts::shadow, Tile{ 13,11 });
			speedy.init(ghostDir, Ghosts::speedy, Tile{ 14,11 });
			bashful.init(ghostDir, Ghosts::bashful, Tile{ 15,11 });
			pokey.init(ghostDir, Ghosts::pokey, Tile{ 12,11 });
		}
		else if (testRegistry.get<Pacman>(player.getEntityID()).dead) {
			std::cout << "Game Over!" << std::endl;
			sounds[2]->play();
			game->delay(1500);
			break;
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
	srand(static_cast<uint32_t>(time(0)));
	namespace fs = std::filesystem;
	fs::path dataDir = fs::path(getExePath()) / L"data";
	fs::path assetsPath = dataDir / L"Assets";
	fs::path musicPath = dataDir / L"Music";
	fs::path soundsPath = dataDir / L"Sounds";

	std::thread drawer_thread{ test_drawer, assetsPath };
	drawer_thread.join();
	sounds.clear();
	return 0;
}