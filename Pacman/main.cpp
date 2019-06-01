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

	FontPtr guiFont = std::make_shared<Font>(FontStyle, 8);

	Level level = Level(game, guiFont);
	int levelNumber = 0;
	level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);

	level.draw();
	game->render();

	sounds.add(audioDir / "pacman_beginning.wav"); // 0 
	sounds.add(audioDir / "pacman_intermission.wav"); // 1
	sounds.add(audioDir / "pacman_death.wav"); // 2
	sounds.add(audioDir / "Pacman Chomp1.wav"); // 3
	sounds.add(audioDir / "Pacman Chomp2.wav"); // 4
	sounds.add(audioDir / "pacman_eatghost.wav"); // 5
	sounds.add(audioDir / "pacman_eatfruit.wav"); // 6
	sounds.add(audioDir / "siren.wav"); // 7 
	sounds.add(audioDir / "siren2.wav"); // 8
	sounds.add(audioDir / "siren3.wav"); // 9
	sounds.add(audioDir / "energizer.wav"); // 10 
	sounds.add(audioDir / "eyes.wav"); // 11

	std::cout << "Level started!" << std::endl;
	sounds[0]->play();
	SDL_Delay(5000);

	auto dt = 0.0;
	int frameCount = 0;
	int chompLoop = 0;
	game->timer.restart();
	int last = 0;
	while (game->isRunning()) {
		dt = game->timer.lap();
		frameCount = (frameCount + 1) % 60;
		game->handleEvents();
		game->clear();
		level.update(dt);
		level.draw();

		if (level.pelletEaten()) sounds[3 + chompLoop]->play(), chompLoop = (chompLoop + 1) % 2;

		if (level.ghostEaten()) sounds[5]->play();

		if (level.bonusEaten()) sounds[6]->play();

		if (level.sirenClip() == 11) {
			if (last == 11 && !level.frozen()) {
				sounds[10]->stop();
				sounds[11]->play(0);
			}
		}
		else {
			if (last != level.sirenClip()) {
				sounds[last]->stop();
				sounds[level.sirenClip()]->play(0);
			}
		}
		last = level.sirenClip();

		game->render();

		if (level.complete()) {
			sounds.stopAll();
			std::cout << "Level complete!" << std::endl;
			sounds[1]->play();
			game->delay(5500);
			++levelNumber;
			level.changeLevel(levelDir / "0.txt", levelNumber, tileDir);
			game->timer.restart();
			last = 0;
		}
		else if (level.dead()) {
			sounds.stopAll();
			std::cout << "Died!" << std::endl;
			sounds[2]->play();
			game->delay(1500);
			last = 0;
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

	std::thread drawer_thread{ test_drawer, assetsPath };
	drawer_thread.join();
	sounds.clear();
	return 0;
}