#include "..\inc\Level.h"

Level::Level(bloom::Game *& gameInstance) : m_gameInstance(gameInstance), m_renderer(gameInstance->getRenderer()) {
	int w, h;
	SDL_GetRendererOutputSize(m_renderer, &w, &h);
	if (m_levelTex)
		SDL_DestroyTexture(m_levelTex);
	m_levelTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
	SDL_SetTextureBlendMode(m_levelTex, SDL_BLENDMODE_BLEND);
}

Level::~Level() {
	SDL_DestroyTexture(m_levelTex);
}

void Level::draw() {
	SDL_RenderCopyEx(m_renderer, m_levelTex, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_NONE);
}

void Level::changeLevel(const std::filesystem::path & levelFile, const std::filesystem::path & texturePath, entt::DefaultRegistry & registry){
	m_pellets.clear();
	m_load(levelFile);
	m_generateTexture(texturePath);
	m_generatePellets(texturePath, registry);
}

void Level::m_load(const std::filesystem::path & levelData) {
	std::ifstream fin(levelData.u8string());

	int w, h;
	fin >> w >> h;

	layout = std::vector<std::vector<int>>(h, std::vector<int>(w));

	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j)
			fin >> layout[i][j];
}

void Level::m_generateTexture(const std::filesystem::path & tilePath) {
	SDL_SetRenderTarget(m_renderer, m_levelTex);
	SDL_RenderClear(m_renderer);
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			if (layout[i][j] != 37 && layout[i][j] != 38) {
				std::filesystem::path tile = tilePath / std::string{ std::to_string(layout[i][j]) + ".png" };
				SDL_Rect src{ 0,0,TILETEXTURESIZE,TILETEXTURESIZE };
				SDL_Rect dest{ j * TILESIZE, i * TILESIZE,TILESIZE,TILESIZE };
				m_gameInstance->textures.load(tile)->render(src, dest);
			}

	SDL_SetRenderTarget(m_renderer, nullptr);
}

void Level::m_generatePellets(const std::filesystem::path & pelletTexturePath, entt::DefaultRegistry & registry) {
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			if (layout[i][j] == 37) {
				auto pellet = std::make_shared<PelletObject>(registry, m_gameInstance);
				pellet->init(pelletTexturePath / std::string{ std::to_string(37) + ".png" }, SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_pellets.emplace_back(pellet);
				layout[i][j] = 0;
			}
			else if (layout[i][j] == 38) {
				auto pellet = std::make_shared<PelletObject>(registry, m_gameInstance);
				pellet->init(pelletTexturePath / std::string{ std::to_string(38) + ".png" }, SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_pellets.emplace_back(pellet);
				layout[i][j] = 0;
			}
}