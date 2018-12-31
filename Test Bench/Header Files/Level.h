#pragma once
#include "Framework.h"
#include "PelletObject.h"
#include "Configs.h"
#include <fstream>

class Level {
public:
	Level(bloom::Game*& gameInstance) : gameInstance(gameInstance), renderer(gameInstance->getRenderer()) {
		int w, h;
		SDL_GetRendererOutputSize(renderer, &w, &h);
		if (levelTex)
			SDL_DestroyTexture(levelTex);
		levelTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
		SDL_SetTextureBlendMode(levelTex, SDL_BLENDMODE_BLEND);
	}
	~Level() {
		SDL_DestroyTexture(levelTex);
	}

	void load(const std::filesystem::path& levelData) {
		std::ifstream fin(levelData.u8string());

		int w, h;
		fin >> w >> h;

		layout = std::vector<std::vector<int>>(h, std::vector<int>(w));

		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
				fin >> layout[i][j];
	}

	void generateTexture(const std::filesystem::path& tilePath) {
		SDL_SetRenderTarget(renderer, levelTex);
		SDL_RenderClear(renderer);
		for (int i = 0; i < layout.size(); ++i)
			for (int j = 0; j < layout[i].size(); ++j)
				if (layout[i][j] != 37 && layout[i][j] != 38) {
					std::filesystem::path tile = tilePath / std::string{ std::to_string(layout[i][j]) + ".png" };
					SDL_Rect src{ 0,0,TILETEXTURESIZE,TILETEXTURESIZE };
					SDL_Rect dest{ j * TILESIZE, i * TILESIZE,TILESIZE,TILESIZE };
					gameInstance->textures.load(tile)->render(src, dest);
				}

		SDL_SetRenderTarget(renderer, nullptr);
	}

	void generatePellets(const std::filesystem::path& pelletTexturePath, entt::DefaultRegistry& registry) {
		for (int i = 0; i < layout.size(); ++i)
			for (int j = 0; j < layout[i].size(); ++j)
				if (layout[i][j] == 37) {
					auto pellet = std::make_shared<PelletObject>(registry, gameInstance);
					pellet->init(pelletTexturePath / std::string{ std::to_string(37) + ".png" }, SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
					pellets.emplace_back(pellet);
					layout[i][j] = 0;
				}
				else if (layout[i][j] == 38) {
					auto pellet = std::make_shared<PelletObject>(registry, gameInstance);
					pellet->init(pelletTexturePath / std::string{ std::to_string(38) + ".png" }, SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
					pellets.emplace_back(pellet);
					layout[i][j] = 0;
				}
	}

	void draw() {
		SDL_RenderCopyEx(renderer, levelTex, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
	}

	bloom::Game*& gameInstance;
	SDL_Renderer* renderer;
	std::vector<std::vector<int>> layout;
	SDL_Texture* levelTex = nullptr;
	std::vector<std::shared_ptr<PelletObject>> pellets;
};