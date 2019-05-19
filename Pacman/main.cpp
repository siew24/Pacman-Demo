#include "Framework.h"
#include <ctime>
#include <Windows.h>

#include <thread>
#include "inc/Configs.h"
#include "inc/Level.h"

#include "getExePath.h"

using namespace bloom;
using namespace bloom::audio;
using namespace bloom::graphics;

Game* game = nullptr;


void test_drawer(const std::filesystem::path& assetsPath)
{
	const int fps = 60;
	const int framedelay = (1000 / fps);

	Uint32 framestart;

	game = new Game(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2, 0, 0 | SDL_RENDERER_TARGETTEXTURE);
	try {
		game->create("Bloom Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	SDL_RenderSetLogicalSize(game->getRenderer(), WINDOW_WIDTH, WINDOW_HEIGHT);

	SDL_Color Background{ 0,0,0 };

	game->setColor(Background);
	game->clear();
	game->render();

	if (!std::filesystem::exists(assetsPath))
		throw bloom::Exception("Required assets can't be found.");

	std::filesystem::path tileDir = assetsPath;
	std::filesystem::path entityDir = assetsPath / L"Entity";
	std::filesystem::path levelDir = assetsPath / L"Level";
	std::filesystem::path audioDir = assetsPath.parent_path() / L"Sounds";
	std::filesystem::path FontDir = assetsPath / L"Font Styles";

	std::filesystem::path FontStyle = FontDir / L"super-mario-bros-nes.ttf";

	if (!std::filesystem::exists(FontStyle))
		throw bloom::Exception("Required assets can't be found.");

	FontPtr guiFont = std::make_shared<Font>(FontStyle, 8);

	SDL_Renderer* renderer = game->getRenderer();



	Level level = Level(game, guiFont);
	int levelNumber = 0;
	level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);

	std::filesystem::path pacDir = assetsPath / L"Pacman.png";
	std::filesystem::path ghostDir = assetsPath;

	level.draw();
	game->render();
	sounds.add(audioDir / "pacman_beginning.wav");
	sounds.add(audioDir / "pacman_intermission.wav");
	sounds.add(audioDir / "pacman_death.wav");

	std::cout << "Level started!" << std::endl;
	sounds[0]->play();
	SDL_Delay(5000);

	auto dt = 0.0;
	int frameCount = 0;

	game->timer.restart();
	while (game->isRunning()) {
		//std::cout << "Delta time: " << dt << "ms" << std::endl;

		dt = game->timer.lap();

		frameCount = (frameCount + 1) % 60;
		game->handleEvents();
		game->clear();
		level.update(dt);
		level.draw();

		game->render();
		// game->update();
		
		if (level.complete()) {
			std::cout << "Level complete!" << std::endl;
			sounds[1]->play();
			game->delay(5500);
			++levelNumber;
			level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);
			game->timer.restart();
		}
		else if (level.dead()) {
			std::cout << "Died!" << std::endl;
			sounds[2]->play();
			game->delay(1500);
			if (level.lives() > 0)
				level.respawn(), game->timer.restart();
			else
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