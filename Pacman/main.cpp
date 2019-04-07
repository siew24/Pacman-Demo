#include "Framework.h"
#include <ctime>
#include <Windows.h>

#include <thread>
#include "inc/Configs.h"
#include "inc/Level.h"

#include "getExePath.h"

using namespace bloom;
using namespace bloom::audio;

Game* game = nullptr;

const int WINDOW_WIDTH = 28 * TILESIZE;
const int WINDOW_HEIGHT = 36 * TILESIZE;

void test_drawer(const std::filesystem::path& assetsPath)
{
	const int fps = 60;
	const int framedelay = (1000 / fps);

	Uint32 framestart;

	game = new Game(WINDOW_WIDTH * 2, WINDOW_HEIGHT * 2, 0, 6 | SDL_RENDERER_TARGETTEXTURE);
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

	Level level = Level(game);
	int levelNumber = 0;
	level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);

	std::filesystem::path pacDir = assetsPath / L"Pacman.png";
	std::filesystem::path ghostDir = assetsPath;

	level.draw();
	game->render();
	sounds.add(audioDir / "pacman_beginning.wav");
	sounds.add(audioDir / "pacman_intermission.wav");
	sounds.add(audioDir / "pacman_death.wav");

	std::cout << std::endl;
	std::string consoleText{ "Level started!" };
	std::cout << consoleText;
	sounds[0]->play();
	SDL_Delay(5000);

	auto dt = 0.0;
	int frameCount = 0;

	while (game->isRunning()) {
		for (char c : consoleText)
			std::cout << "\b";
		consoleText = "[FPS:" + std::to_string(static_cast<int>(1000.0 / dt + 0.5 > 99 ? 99 : 1000.0 / dt + 0.5)) + "] ";
		consoleText += "Current score:  " + std::to_string(level.getScore());
		std::cout << consoleText;

		//std::cout << "Delta time: " << dt << "ms" << std::endl;

		frameCount = (frameCount + 1) % 60;
		game->handleEvents();
		using namespace bloom::input;

		if (game->input.keyboard.wasDown(KeyboardKey::KEY_W) || game->input.keyboard.wasDown(KeyboardKey::KEY_UP))
			level.changeDir(Direction::up);
		else if (game->input.keyboard.wasDown(KeyboardKey::KEY_A) || game->input.keyboard.wasDown(KeyboardKey::KEY_LEFT))
			level.changeDir(Direction::left);
		else if (game->input.keyboard.wasDown(KeyboardKey::KEY_S) || game->input.keyboard.wasDown(KeyboardKey::KEY_DOWN))
			level.changeDir(Direction::down);
		else if (game->input.keyboard.wasDown(KeyboardKey::KEY_D) || game->input.keyboard.wasDown(KeyboardKey::KEY_RIGHT))
			level.changeDir(Direction::right);
		/*else
			level.changeDir(testRegistry, null);*/

		game->clear();
		level.update(dt);
		level.draw();
		game->render();
		// game->update();
		dt = game->timer.lap();

		if (level.complete()) {
			consoleText += " -- Level complete!";
			std::cout << " -- Level complete!";
			sounds[1]->play();
			game->delay(5500);
			++levelNumber;
			level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);
		}
		else if (level.gameOver()) {
			consoleText += " -- Game over!";
			std::cout << " -- Game Over!";
			sounds[2]->play();
			game->delay(1500);
			break;
		}
	}
	std::cout << std::endl;
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