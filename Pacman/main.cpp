#include "Framework.h"

#include <thread>
#include "inc/Configs.h"
#include "inc/Level.h"
#include "inc/Scenes/MainMenu.h"
#include "inc/Scenes/Leaderboards.h"
#include "inc/Scenes/Options.h"
#include "inc/Scenes/ScoreSubmit.h"
#include "inc/Scenes/About.h"
#include "inc/Scenes/SelectLevels.h"
#include "inc/Scenes/LevelList.h"
#include "inc/LevelCreator/LevelCreator.h"

#include "getExePath.h"

#include "inc/ConfigStore.h"

using namespace bloom;
using namespace bloom::audio;
using namespace bloom::graphics;

Game* game = nullptr;

void test_drawer(const std::filesystem::path& assetsPath)
{
	std::filesystem::path appData;
	{
		char* pValue;
		size_t len;
		errno_t err = _dupenv_s(&pValue, &len, "APPDATA");
		appData = std::filesystem::path(pValue);
	}

	std::filesystem::path configPath = appData / L"BF Pacman" / L"configs.ini";
	std::filesystem::path leaderboardsPath = appData / L"BF Pacman" / L"leaderboards.db";

	if (std::filesystem::exists(configPath.parent_path())) {
		if (std::filesystem::exists(configPath)) {
			std::ifstream config(configPath);
			double verNo;
			config >> verNo;
			if (verNo == VERSION_NUMBER) {
				config >> ConfigStore::ghostVolume;
				config >> ConfigStore::pacmanVolume;
				config >> ConfigStore::musicVolume;
				config >> ConfigStore::instaQuit;
				config >> ConfigStore::debug;
			}
		}
		if (std::filesystem::exists(leaderboardsPath)) {
			std::ifstream leaderboards(leaderboardsPath);
			while (!leaderboards.eof()) {
				std::pair<std::string, int> pair;
				leaderboards >> pair.first >> pair.second;
				if (leaderboards.eof()) break;
				LeaderboardsStore::leaderboards.emplace_back(pair);
			}
		}
	}
	else {
		std::filesystem::create_directory(configPath.parent_path());
	}

	game = new Game({ ((2 * TILESIZE) + WINDOW_WIDTH) * 2, ((2 * TILESIZE) + WINDOW_HEIGHT) * 2 }, { 0, 0 | SDL_RENDERER_TARGETTEXTURE });
	try {
		game->create("BF Pacman", { SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED });
	}
	catch (Exception & e) {
		std::cerr << e.what() << std::endl;
	}

	SDL_RenderSetLogicalSize(game->_getRenderer(), (2 * TILESIZE) + WINDOW_WIDTH, (2 * TILESIZE) + WINDOW_HEIGHT);

	SDL_Color Background{ 0,0,0 };

	game->setColor(Background);
	game->clear();
	game->render();

	if (!std::filesystem::exists(assetsPath))
		throw bloom::Exception("Required assets can't be found.");

	std::filesystem::path tileDir = assetsPath;
	std::filesystem::path entityDir = assetsPath / L"Entity";
	std::filesystem::path levelDir = assetsPath / L"Level";
	std::filesystem::path customLevelsPath = appData / L"BF Pacman" / L"Custom Levels";
	std::filesystem::path audioDir = assetsPath.parent_path() / L"Sounds";
	std::filesystem::path FontDir = assetsPath / L"Font Styles";
	std::filesystem::path FontStyle = FontDir / L"super-mario-bros-nes.ttf";

	std::vector<std::filesystem::path> officialLevels;
	if (!std::filesystem::is_empty(levelDir))
		for (const auto& entry : std::filesystem::directory_iterator(levelDir))
			if (entry.is_regular_file())
				officialLevels.emplace_back(entry.path());


	FontPtr guiFont = std::make_shared<Font>(FontStyle, 8);
	MainMenu menu(game, guiFont);
	int scorePos = -1;
	while (game->isRunning()) {
		if (menu.selected == 5) {
			break;
		}
		else if (menu.selected == 4) {
			menu.selected = -1;
			About about(game, guiFont);
			while (about.selected != 1 && game->isRunning()) {
				game->handleEvents();
				game->clear();
				about.update();
				about.draw();
				game->render();
			}
		}
		else if (menu.selected == 3) {
			menu.selected = -1;
			Options options(game, guiFont);
			while (options.selected != 1 && game->isRunning()) {
				game->handleEvents();
				game->clear();
				options.update();
				options.draw();
				game->render();
			}
		}
		else if (menu.selected == 2) {
			menu.selected = -1;
			Leaderboards leaderboards(game, guiFont, scorePos);
			while (leaderboards.selected != 1 && game->isRunning()) {
				game->handleEvents();
				game->clear();
				leaderboards.update();
				leaderboards.draw();
				game->render();
			}
			scorePos = -1;
		}
		else if (menu.selected == 1) {
			menu.selected = -1;
			LevelCreator levelCreator(game, guiFont);
			while (levelCreator.selected != 1 && game->isRunning()) {
				game->handleEvents();
				game->clear();
				levelCreator.update(game->timer.lap());
				levelCreator.draw();
				game->render();
			}
		}
		else if (menu.selected == 0) {
			menu.selected = -1;
			std::vector<std::filesystem::path> levelSet;
			{
				SelectLevel levelSelect(game, guiFont);
				while (levelSelect.selected == -1 && game->isRunning()) {
					game->handleEvents();
					game->clear();
					levelSelect.update();
					levelSelect.draw();
					game->render();
				}
				if (levelSelect.selected == 1) {
					levelSet = officialLevels;
				}
				else if (levelSelect.selected == 2) {
					std::vector<std::filesystem::path> customLevels;
					if (!std::filesystem::is_empty(customLevelsPath))
						for (const auto& entry : std::filesystem::directory_iterator(customLevelsPath))
							if (entry.is_regular_file())
								customLevels.emplace_back(entry.path());
					LevelList levelSelect(game, guiFont, customLevels);
					while (levelSelect.selected == -1 && game->isRunning()) {
						game->handleEvents();
						game->clear();
						levelSelect.update();
						levelSelect.draw();
						game->render();
					}
					if (levelSelect.selected == 1)
						levelSet.emplace_back(levelSelect.selectedPath);
				}
			}
			auto levelIt = levelSet.begin();
			if (!levelSet.empty()) {
				game->clear();
				Level level = Level(game, guiFont);
				int levelNumber = 0;
				level.changeLevel(*levelIt, levelNumber, tileDir);

				/*level.draw();
				game->render();*/

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

				for (int i = 0; i < 12; ++i) {
					if (i < 3) sounds[i]->setVolume(ConfigStore::musicVolume);
					else if (i < 7) sounds[i]->setVolume(ConfigStore::pacmanVolume);
					else sounds[i]->setVolume(ConfigStore::ghostVolume);
				}

				std::cout << "Level started!" << std::endl;
				bloom::Timer timer;
				timer.restart();
				sounds[0]->play();
				while (timer.split() <= 5000.0 && game->isRunning()) {
					level.ready();
					game->handleEvents();
					game->render();
				}

				auto dt = 0.0;
				int frameCount = 0;
				int chompLoop = 0;
				game->timer.restart();
				int last = 0;
				bool finished = false;
				bool quit = false;
				int count1 = 0, count2 = 0;
				auto updateLoop = [&] {
					bloom::Timer threadTimer;
					double dt;
					while (!quit && (!level.complete() && !level.dead() && !level.quit())) {
						dt = threadTimer.lap();
						++count1;

						level.update(dt);
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
					}
				};
				std::thread updatethread = std::thread(updateLoop);
				std::thread printStats = std::thread([&] {
					while (!quit) {
						std::cout << count1 << ", " << count2 << std::endl;
					}
					}
				);

				while (game->isRunning()) {
					dt = game->timer.lap();
					level.drawFPS = 1000.0 / dt;
					++count2;
					frameCount = (frameCount + 1) % 60;
					game->handleEvents();
					level.handleInput(dt);
					game->clear();
					level.draw();
					game->render();

					if (level.complete()) {
						updatethread.join();
						sounds.stopAll();
						std::cout << "Level complete!" << std::endl;
						level.finish();
						++levelNumber;
						++levelIt;
						if (levelIt == levelSet.end()) {
							if (levelSet[0].parent_path() == levelDir) {
								levelIt = levelSet.begin();
							}
							else
								break;
						}
						level.changeLevel(*levelIt, levelNumber, tileDir);
						timer.restart();
						game->clear();
						while (timer.split() <= 3000.0 && game->isRunning()) {
							level.ready();
							game->handleEvents();
							game->render();
						}
						game->timer.restart();
						last = 0;
						updatethread = std::thread(updateLoop);
					}
					else if (level.dead()) {
						updatethread.join();
						sounds.stopAll();
						std::cout << "Died!" << std::endl;
						timer.restart();
						sounds[2]->play();
						while (timer.split() <= 1500.0 && game->isRunning()) {
							game->handleEvents();
							game->render();
						}

						last = 0;
						if (level.lives() > 0) {
							level.respawn();
							timer.restart();
							game->clear();
							while (timer.split() <= 3000.0 && game->isRunning()) {
								level.ready();
								game->handleEvents();
								game->render();
							}
							game->timer.restart();
							updatethread = std::thread(updateLoop);
						}
						else {
							if (levelSet[0].parent_path() == levelDir) {
								ScoreSubmit scoresubmit(game, guiFont, level.getScore());
								while (scoresubmit.selected == -1 && game->isRunning()) {
									game->handleEvents();
									game->clear();
									scoresubmit.update();
									scoresubmit.draw();
									game->render();
								}

								if (scoresubmit.selected >= 0) {
									menu.selected = 2;
									scorePos = scoresubmit.selected;
								}
							}
							finished = true;
							break;
						}
					}
					else if (level.quit()) {
						updatethread.join();
						sounds.stopAll();
						break;
					}
					else if (!game->isRunning()) {
						quit = true;
						updatethread.join();
					}
				}
				quit = true;
				printStats.join();
				if (updatethread.joinable())
					updatethread.join();
			}
		}
		else if (menu.selected == -1) {
			game->handleEvents();
			game->clear();
			menu.update();
			menu.draw();
			game->render();
		}
	}

	game->destroy();

	std::ofstream fout(configPath);
	fout << VERSION_NUMBER << std::endl
		<< ConfigStore::ghostVolume << std::endl
		<< ConfigStore::pacmanVolume << std::endl
		<< ConfigStore::musicVolume << std::endl
		<< ConfigStore::instaQuit << std::endl
		<< ConfigStore::debug;

	fout = std::ofstream(leaderboardsPath);
	for (auto& scoreEntry : LeaderboardsStore::leaderboards) {
		fout << scoreEntry.first << " " << scoreEntry.second << std::endl;
	}
}

int main(int argc, char* argv[])
{
	namespace fs = std::filesystem;
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

	fs::path dataDir = fs::path(getExePath()) / L"data";
	fs::path assetsPath = dataDir / L"Assets";
	std::thread drawer_thread{ test_drawer, assetsPath };
	drawer_thread.join();
	sounds.clear();
	return 0;
}