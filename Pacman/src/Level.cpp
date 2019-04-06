#include "..\inc\Level.h"
#include "..\inc\GhostObject.h"


Level::Level(bloom::Game *& gameInstance) : m_gameInstance(gameInstance), m_renderer(gameInstance->getRenderer()) {
	if (m_levelTex)
		SDL_DestroyTexture(m_levelTex);
	if (m_entityLayer)
		SDL_DestroyTexture(m_entityLayer);

	m_levelTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28*TILESIZE), (31*TILESIZE));
	m_entityLayer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28*TILESIZE), (31*TILESIZE));
	
	SDL_SetTextureBlendMode(m_levelTex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(m_entityLayer, SDL_BLENDMODE_BLEND);

	gameDirector.setParameters(gameInstance);
}

Level::~Level() {
	SDL_DestroyTexture(m_levelTex);
	SDL_DestroyTexture(m_entityLayer);
	SDL_RenderClear(m_renderer);
}

void Level::draw() {
	SDL_SetRenderTarget(m_renderer, m_entityLayer);
	SDL_RenderClear(m_renderer);
	renderSysTest.update(); // Test again.
	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopyEx(m_renderer, m_levelTex, nullptr, &GAMEAREA, 0.0, nullptr, SDL_FLIP_NONE);
	SDL_RenderCopyEx(m_renderer, m_entityLayer, nullptr, &GAMEAREA, 0.0, nullptr, SDL_FLIP_NONE);
}

void Level::changeLevel(const std::filesystem::path & levelFile, int levelNumber, const std::filesystem::path & texturePath){
	m_entities.clear();
	m_load(levelFile);
	m_generateTexture(texturePath);
	m_generateEntities(levelFile.parent_path(),  levelNumber, texturePath);
	m_cleanup();

	playerMovement.layout = layout;
	ghostMovement.layout = layout;
	speedDirector.layout = layout;
	
	FruitType fruit;
	{
		std::ifstream fin(levelFile.parent_path() / "LevelFruits.txt");
		int n;
		fin >> n;
		for (int i = 0; i < n ; ++i) {
			int score;
			fin >> score;
			fruit = static_cast<FruitType>(score);
			if (i == levelNumber)
				break;
		}
	}
	gameDirector.setParameters(texturePath, fruit);
	gameDirector.init();
	speedDirector.init();

	animSysTest.update(0);
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
void Level::m_generateEntities(const std::filesystem::path& TimeDataPath, int levelNumber, const std::filesystem::path & texturePath) {
	GhostInitDetails ghostDet;
	std::ifstream fin(TimeDataPath / "TimerData.txt");
	int entries;
	fin >> entries;
	int l, r;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 8; ++j)
			fin >> ghostDet.modeTimes[j];

		if (levelNumber >= l && levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(TimeDataPath / "PacmanSpeed.txt");
	fin >> entries;
	std::array<double, 2> pacmanSpeeds;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 2; ++j)
			fin >> pacmanSpeeds[j];

		if (levelNumber >= l && levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(TimeDataPath / "AfraidFlash.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		fin >> ghostDet.flashAmount;

		if (levelNumber >= l && levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(TimeDataPath / "FrightTimes.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> ghostDet.afraidTime;
		if (i == levelNumber)
			break;
	}

	fin = std::ifstream(TimeDataPath / "GhostSpeed.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 3; ++j)
			fin >> ghostDet.multipliers[j];

		if (levelNumber >= l && levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(TimeDataPath / "ElroySpeed.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		std::pair<int, double> tmp;
		for (int j = 0; j < 2; ++j) {
			fin >> tmp.first >> tmp.second;
			ghostDet.elroyMultipliers[j] = tmp;
		}

		if (levelNumber >= l && levelNumber < r || r == -1)
			break;
	}

	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1) == 1) {
				auto player = std::make_shared<Player>(m_registry, m_gameInstance);
				player->init(texturePath / "Entity" / "Pacman.png", Tile{ j,i }, pacmanSpeeds);
				m_entities.emplace_back(player);
				playerEntity = player;
				layout[i][j] = -(-layout[i][j] | 1);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 2) == 2) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(texturePath / "Entity", Ghosts::shadow, Tile{ j,i }, ghostDet);
				m_entities.emplace_back(ghost);
				layout[i][j] = -(-layout[i][j] | 2);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 4) == 4) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(texturePath / "Entity", Ghosts::speedy, Tile{ j,i }, ghostDet);
				m_entities.emplace_back(ghost);
				layout[i][j] = -(-layout[i][j] | 4);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 8) == 8) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(texturePath / "Entity", Ghosts::bashful, Tile{ j,i }, ghostDet);
				m_entities.emplace_back(ghost);
				layout[i][j] = -(-layout[i][j] | 8);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 16) == 16) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(texturePath / "Entity", Ghosts::pokey, Tile{ j,i }, ghostDet);
				m_entities.emplace_back(ghost);
				layout[i][j] = -(-layout[i][j] | 16);
			}
		}

	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && ((std::abs(layout[i][j]) & 512) == 512)) {
				auto pellet = std::make_shared<PelletObject>(m_registry, m_gameInstance);
				pellet->init(texturePath / "Entity" / "Pellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_entities.emplace_back(pellet);
				layout[i][j] = -(-layout[i][j] | 512);
				++m_pellets;
			}if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1024) == 1024) {
				auto pellet = std::make_shared<PelletObject>(m_registry, m_gameInstance);
				pellet->init(texturePath / "Entity" / "PowerPellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_entities.emplace_back(pellet);
				layout[i][j] = -(-layout[i][j] | 1024);
				++m_pellets;
			}
		}
}

void Level::m_cleanup() {
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			layout[i][j] = (layout[i][j] < 0 && (std::abs(layout[i][j]) & 256) != 256) ? 0 : layout[i][j];
}