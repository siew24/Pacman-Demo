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

class Level {
public:
	Level(bloom::Game*& gameInstance);
	~Level();

	void changeLevel(const std::filesystem::path& levelFile,int levelNumber, const std::filesystem::path& texturePath);

	void draw();
	size_t pelletCount() {return m_pellets;}
	bool complete() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).pelletsEaten == m_pellets;
	}
	bool gameOver() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).dead;
	}
	void changeDir(Direction dir) {
		m_registry.get<Pacman>(playerEntity->getEntityID()).nextDir = dir;
	}
	int getScore() {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).score;
	}
	void update(double dt) {
		playerMovement.update(dt);
		ghostMovement.update(dt);
		edibleSystem.update(dt);
		speedDirector.update(dt);
		gameDirector.update(dt);
		animSysTest.update(dt);
	}


	std::vector<std::vector<int>> layout;

private:
	void m_load(const std::filesystem::path& levelData);
	void m_generateTexture(const std::filesystem::path& tilePath);
	void m_generateEntities(const std::filesystem::path& TimeData, int levelNumber, const std::filesystem::path& pelletTexturePath);
	void m_cleanup();


	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	SDL_Texture* m_entityLayer = nullptr;
	std::vector<std::shared_ptr<bloom::GameObject>> m_entities;
	std::shared_ptr<Player> playerEntity;
	std::array<double, 8> m_ghostTimes;

	entt::DefaultRegistry m_registry;
	bloom::systems::RenderSystem renderSysTest = bloom::systems::RenderSystem(m_registry);
	AltAnimationSystem animSysTest = AltAnimationSystem(m_registry);
	PlayerMovement playerMovement= PlayerMovement(m_registry);
	GhostAI ghostMovement = GhostAI(m_registry);
	EdibleSystem edibleSystem = EdibleSystem(m_registry);
	GameDirectorSystem gameDirector = GameDirectorSystem(m_registry);
	SpeedDirectorSystem speedDirector = SpeedDirectorSystem(m_registry);

	int m_pellets = 0;
};