#pragma once

#include "Game.h"
#include "PelletObject.h"
#include "FruitObject.h"
#include "Configs.h"
#include <fstream>
#include "PlayerObject.h"

class Level {
public:
	Level(bloom::Game*& gameInstance);
	~Level();

	void changeLevel(const std::filesystem::path& levelFile, const std::filesystem::path& texturePath, entt::DefaultRegistry& registry);

	void draw();
	size_t pelletCount() {return m_pellets;}
	bool complete(entt::DefaultRegistry& registry) {
		return registry.get<Pacman>(playerEntity->getEntityID()).pelletsEaten == m_pellets;
	}
	bool gameOver(entt::DefaultRegistry& registry) {
		return registry.get<Pacman>(playerEntity->getEntityID()).dead;
	}
	void changeDir(entt::DefaultRegistry& registry, Direction dir) {
		registry.get<Pacman>(playerEntity->getEntityID()).nextDir = dir;
	}
	int getScore(entt::DefaultRegistry& registry) {
		return registry.get<Pacman>(playerEntity->getEntityID()).score;
	}


	std::vector<std::vector<int>> layout;

private:
	void m_load(const std::filesystem::path& levelData);
	void m_generateTexture(const std::filesystem::path& tilePath);
	void m_generateEntities(const std::filesystem::path& pelletTexturePath, entt::DefaultRegistry& registry);
	void m_cleanup();


	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	std::vector<std::shared_ptr<bloom::GameObject>> m_entities;
	std::shared_ptr<Player> playerEntity;

	int m_pellets = 0;
};