#include "..\inc\Level.h"
#include "..\inc\GhostObject.h"


Level::Level(bloom::Game *& gameInstance) : m_gameInstance(gameInstance), m_renderer(gameInstance->getRenderer()) {
	int w, h;
	SDL_RenderGetLogicalSize(m_renderer, &w, &h);
	if (w == 0 || h == 0)
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
	m_entities.clear();
	m_load(levelFile);
	m_generateTexture(texturePath);
	m_generateEntities(texturePath, registry);
	m_cleanup();
}

void Level::m_load(const std::filesystem::path & levelData) {
	std::ifstream fin(levelData.u8string());

	int w, h;
	fin >> w >> h;

	layout = std::vector<std::vector<int>>(h, std::vector<int>(w));

	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j)
			fin >> std::hex >> layout[i][j];
}

void Level::m_generateTexture(const std::filesystem::path & tilePath) {
	SDL_SetRenderTarget(m_renderer, m_levelTex);
	SDL_RenderClear(m_renderer);
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			if (layout[i][j] > 0) {
				std::filesystem::path tile = tilePath / "Tile" / std::string{ std::to_string(layout[i][j]) + ".png" };
				SDL_Rect src{ 0,0,TILETEXTURESIZE,TILETEXTURESIZE };
				SDL_Rect dest{ j * TILESIZE, i * TILESIZE,TILESIZE,TILESIZE };
				m_gameInstance->textures.load(tile)->render(src, dest);
			}

	SDL_SetRenderTarget(m_renderer, nullptr);
}
void Level::m_generateEntities(const std::filesystem::path & texturePath, entt::DefaultRegistry & registry) {
	

	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1) == 1) {
				auto player = std::make_shared<Player>(registry, m_gameInstance);
				player->init(texturePath / "Entity" / "Pacman.png", Tile{ j,i });
				m_entities.emplace_back(player);
				playerEntity = player;
				layout[i][j] |= 1;
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 2) == 2) {
				auto ghost = std::make_shared<GhostObject>(registry, m_gameInstance);
				ghost->init(texturePath / "Entity", shadow, Tile{ j,i });
				m_entities.emplace_back(ghost);
				layout[i][j] |= 2;
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 4) == 4) {
				auto ghost = std::make_shared<GhostObject>(registry, m_gameInstance);
				ghost->init(texturePath / "Entity", speedy, Tile{ j,i });
				m_entities.emplace_back(ghost);
				layout[i][j] |= 4;
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 8) == 8) {
				auto ghost = std::make_shared<GhostObject>(registry, m_gameInstance);
				ghost->init(texturePath / "Entity", bashful, Tile{ j,i });
				m_entities.emplace_back(ghost);
				layout[i][j] |= 8;
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 16) == 16) {
				auto ghost = std::make_shared<GhostObject>(registry, m_gameInstance);
				ghost->init(texturePath / "Entity", pokey, Tile{ j,i });
				m_entities.emplace_back(ghost);
				layout[i][j] |= 16;
			}
		}

	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && ((std::abs(layout[i][j]) & 512) == 512)) {
				auto pellet = std::make_shared<PelletObject>(registry, m_gameInstance);
				pellet->init(texturePath / "Entity" / "Pellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_entities.emplace_back(pellet);
				layout[i][j] |= 512;
				++m_pellets;
			}if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1024) == 1024) {
				auto pellet = std::make_shared<PelletObject>(registry, m_gameInstance);
				pellet->init(texturePath / "Entity" / "PowerPellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_entities.emplace_back(pellet);
				layout[i][j] |= 1024;
				++m_pellets;
			}
		}
}

void Level::m_cleanup() {
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			layout[i][j] = layout[i][j] < 0 ? 0 : layout[i][j];
}