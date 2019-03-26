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
#include "Systems/PelletSystem.h"
#include "Systems/FruitSystem.h"
#include "Systems/GhostAI.h"
#include "Systems/GameDirectorSystem.h"
#include "Systems/SpeedDirectorSystem.h"

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
	int getScore(entt::DefaultRegistry& m_registry) {
		return m_registry.get<Pacman>(playerEntity->getEntityID()).score;
	}
	void update(double dt) {
		playerMovement.update(dt);
		ghostMovement.update(dt);
		pelletSystem.update();
		fruitSystem.update(dt);
		speedDirector.update();
		gameDirector.update();
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
	bloom::systems::AnimationSystem animSysTest = bloom::systems::AnimationSystem(m_registry);
	PlayerMovement playerMovement= PlayerMovement(m_registry);
	GhostAI ghostMovement = GhostAI(m_registry);
	PelletSystem pelletSystem = PelletSystem(m_registry);
	FruitSystem fruitSystem = FruitSystem(m_registry);
	GameDirectorSystem gameDirector = GameDirectorSystem(m_registry);
	SpeedDirectorSystem speedDirector = SpeedDirectorSystem(m_registry);

	int m_pellets = 0;
};