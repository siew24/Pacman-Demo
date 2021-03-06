#pragma once

#include "Game.h"
#include "PelletObject.h"
#include "FruitObject.h"
#include "Configs.h"
#include <fstream>
#include "PlayerObject.h"
#include "entt/entt.hpp"
#include "PlayerObject.h"
#include "Systems/PlayerMovement.h"
#include "Systems/EdibleSystem.h"
#include "Systems/GhostAI.h"
#include "Systems/GameDirectorSystem.h"
#include "Systems/SpeedDirectorSystem.h"
#include "Systems/AltAnimationSystem.h"
#include "Systems/AltRenderSystem.h"
#include "Systems/ScorePopupSystem.h"
#include "Systems/AnimationChangerSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/SirenSystem.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"

class GhostObject;

class Level {
public:
	Level(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont);
	~Level();

	void changeLevel(const std::filesystem::path& levelFile, int levelNumber, const std::filesystem::path& texturePath);

	void draw(SDL_Texture* levelTex = nullptr);
	size_t pelletCount() {
		return m_registry.view<Pellet>().size() + m_registry.view<PowerPellet>().size();
	}
	void ready() {
		draw();
		guiElems["readyText"]->render(std::nullopt, SDL_Point{ 10 * TILESIZE,21 * TILESIZE });
	}
	void finish() {
		double timeLeft = 3000.0;
		auto& timer = m_gameInstance->timer;
		timer.restart();
		double frameTime = 0.0;
		SDL_Texture* levelTex = m_levelTex;
		while (timeLeft > 0.0) {
			m_gameInstance->handleEvents();
			double dt = timer.lap();
			timeLeft -= dt;
			if (timeLeft <= 2000.0)
				frameTime += dt;
			while (frameTime > 333.0) {
				if (levelTex == m_levelTex)
					levelTex = m_wLevelTex;
				else
					levelTex = m_levelTex;
				frameTime -= 333.0;
			}
			m_gameInstance->clear();
			draw(levelTex);
			m_gameInstance->render();
		}
	}
	bool complete() {
		return (m_registry.get<Pacman>(playerEntity->getEntityID()).totalPellets == pelletCount());
	}
	bool dead() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).dead;
	}
	int lives() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).lives;
	}
	int getScore() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).score;
	}
	void refreshTexture() {
		m_generateTexture();
	}
	bool pelletEaten() {
		bool returnVal = edibleSystem.pelletEaten;
		edibleSystem.pelletEaten = false;
		return returnVal;
	}
	bool bonusEaten() {
		bool returnVal = edibleSystem.bonusEaten;
		edibleSystem.bonusEaten = false;
		return returnVal;
	}
	bool ghostEaten() {
		bool returnVal = edibleSystem.ghostEaten;
		edibleSystem.ghostEaten = false;
		return returnVal;
	}
	int sirenClip() {
		return sirenHandler.soundbyte;
	}
	bool frozen() {
		return popupSystem.freeze;
	}
	void respawn();
	void handleInput(double dt) {
		inputHandler.update(dt);
		if (inputHandler.quit)
			m_quit = true;
	}
	void update(double dt) {
		if (dt > 0.0) {
			++totalFrames;
			totalTime += dt / 1000.0;
			current += dt;
		}

		popupSystem.update(dt);
		if (!popupSystem.freeze) {
			playerMovement.update(dt);
			ghostMovement.update(dt);
			speedDirector.update(dt);
			gameDirector.update(dt);
			animationChanger.update(dt);
			edibleSystem.update(dt);
			animSysTest.update(dt);
			sirenHandler.update();
		}
		guiElems["score"]->setText(std::to_string(getScore()));
		if (LeaderboardsStore::leaderboards.empty())
			guiElems["highscore"]->setText(std::to_string(getScore()));
		else
			guiElems["highscore"]->setText(std::to_string(((getScore() > LeaderboardsStore::leaderboards.front().second) ? getScore() : LeaderboardsStore::leaderboards.front().second)));
		if (ConfigStore::debug && totalFrames > 0 && totalTime > 0.0 && current / 100.0 >= 1.0) {
			current = std::fmod(current, 100.0);
			guiElems["FPS"]->setText(std::to_string(static_cast<int>((1000.0 / dt) + 0.5)));
			guiElems["avgFPS"]->setText(std::to_string(drawFPS));
		}
	}
	bool quit() {
		return m_quit;
	}
	std::vector<std::vector<int>> layout;
	int drawFPS = 0;

private:
	void m_load(const std::filesystem::path& levelData);
	void m_generateTexture();

	void m_generatePellets(std::vector<std::vector<int>>& layout, bool readOnly = true);
	void m_generateEntities(std::vector<std::vector<int>>& layout, bool readOnly = true);
	void m_cleanup();


	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	SDL_Texture* m_wLevelTex = nullptr;
	SDL_Texture* m_entityLayer = nullptr;
	std::vector<std::shared_ptr<PelletObject>> m_pellets;
	std::vector<std::shared_ptr<GhostObject>> m_ghosts;
	std::shared_ptr<Player> playerEntity;
	std::array<double, 8> m_ghostTimes;
	std::array<std::array<int, 31>, 28> m_originalLayout;

	entt::registry m_registry;
	AltRenderSystem renderSysTest = AltRenderSystem(m_registry);
	AltAnimationSystem animSysTest = AltAnimationSystem(m_registry);
	PlayerMovement playerMovement = PlayerMovement(m_registry);
	GhostAI ghostMovement = GhostAI(m_registry);
	EdibleSystem edibleSystem = EdibleSystem(m_registry);
	GameDirectorSystem gameDirector = GameDirectorSystem(m_registry);
	SpeedDirectorSystem speedDirector = SpeedDirectorSystem(m_registry);
	ScorePopupSystem popupSystem = ScorePopupSystem(m_registry);
	AnimationChangerSystem animationChanger = AnimationChangerSystem(m_registry);
	InputHandlerSystem inputHandler = InputHandlerSystem(m_registry, m_gameInstance->input);
	SirenSystem sirenHandler = SirenSystem(m_registry);

	std::filesystem::path m_levelFile;
	int m_levelNumber;
	std::filesystem::path m_texturePath;

	// GUI Text
	std::unordered_map<std::string, std::shared_ptr<bloom::graphics::SpriteText>> guiElems;

	// Pause Variable
	double pauseTimer = 0;

	double totalTime = 0.0;
	int totalFrames = -1;

	double current = 0.0;

	bool m_quit = false;

	std::array<std::array<int, 31>, 28> tileMap;
	std::array<std::array<int, 31>, 28> specialMap;
};
