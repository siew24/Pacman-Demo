#pragma once

#include "Game.h"
#include "PelletObject.h"
#include "Configs.h"
#include <fstream>

class Level {
public:
	Level(bloom::Game*& gameInstance);
	~Level();

	void changeLevel(const std::filesystem::path& levelFile, const std::filesystem::path& texturePath, entt::DefaultRegistry& registry);

	void draw();
	size_t pelletCount() {return m_pellets.size();}

	std::vector<std::vector<int>> layout;

private:
	void m_load(const std::filesystem::path& levelData);
	void m_generateTexture(const std::filesystem::path& tilePath);
	void m_generatePellets(const std::filesystem::path& pelletTexturePath, entt::DefaultRegistry& registry);

	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	std::vector<std::shared_ptr<PelletObject>> m_pellets;
};