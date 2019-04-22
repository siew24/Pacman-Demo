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
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"

class Level {
public:
	Level(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont);
	~Level();

	void changeLevel(const std::filesystem::path& levelFile, int levelNumber, const std::filesystem::path& texturePath);

	void draw();
	size_t pelletCount() { return m_pellets.size(); }
	bool complete() {
		return (m_registry.view<Pellet>().empty() && m_registry.view<PowerPellet>().empty());
	}
	bool dead() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).dead;
	}
	int lives() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).lives;
	}
	void changeDir(Direction dir) {
		m_registry.get<Pacman>(playerEntity->getEntityID()).nextDir = dir;
	}
	int getScore() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).score;
	}
	void refreshTexture() {
		m_generateTexture();
	}
	void respawn();
	void update(double dt) {
		if (!isPaused(dt)) {
			playerMovement.update(dt);
			ghostMovement.update(dt);
			edibleSystem.update(dt);
			speedDirector.update(dt);
			gameDirector.update(dt);
			animSysTest.update(dt);
		}
		guiElems[0].setText(std::to_string(getScore()));
		guiElems[1].setText(std::to_string(static_cast<int>(1000.0 / dt + 0.5)));
	}
	bool isPaused(double dt) {
		pauseTimer += (ghostMovement.isPaused) ? dt : 0;
		if (pauseTimer >= 1000.0)
			ghostMovement.isPaused = false, pauseTimer = 0;

		return ghostMovement.isPaused;
	}

	std::vector<std::vector<int>> layout;

private:
	void m_load(const std::filesystem::path& levelData);
	void m_generateTexture();

	void m_generatePellets(std::vector<std::vector<int>>& layout, bool readOnly = true);
	void m_generateEntities(std::vector<std::vector<int>>& layout, bool readOnly = true);
	void m_cleanup();


	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	SDL_Texture* m_entityLayer = nullptr;
	std::vector<std::shared_ptr<bloom::GameObject>> m_pellets;
	std::vector<std::shared_ptr<bloom::GameObject>> m_ghosts;
	std::shared_ptr<Player> playerEntity;
	std::array<double, 8> m_ghostTimes;
	std::vector<std::vector<int>> m_originalLayout;

	entt::DefaultRegistry m_registry;
	bloom::systems::RenderSystem renderSysTest = bloom::systems::RenderSystem(m_registry);
	AltAnimationSystem animSysTest = AltAnimationSystem(m_registry);
	PlayerMovement playerMovement = PlayerMovement(m_registry);
	GhostAI ghostMovement = GhostAI(m_registry);
	EdibleSystem edibleSystem = EdibleSystem(m_registry);
	GameDirectorSystem gameDirector = GameDirectorSystem(m_registry);
	SpeedDirectorSystem speedDirector = SpeedDirectorSystem(m_registry);

	std::filesystem::path m_levelFile;
	int m_levelNumber; 
	std::filesystem::path m_texturePath;

	// GUI Text
	std::vector<bloom::graphics::SpriteText> guiElems;

	// Pause Variable
	double pauseTimer = 0;
};
