#include "..\inc\Level.h"
#include "..\inc\GhostObject.h"


Level::Level(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance), m_renderer(gameInstance->getRenderer()) {
	if (m_levelTex)
		SDL_DestroyTexture(m_levelTex);
	if (m_entityLayer)
		SDL_DestroyTexture(m_entityLayer);

	m_levelTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28 * TILESIZE), (33 * TILESIZE));
	m_entityLayer = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28 * TILESIZE), (33 * TILESIZE));

	SDL_SetTextureBlendMode(m_levelTex, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(m_entityLayer, SDL_BLENDMODE_BLEND);

	gameDirector.setParameters(gameInstance);
	guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "0"));
	guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "60"));
	guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "1UP"));
	guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "HIGH SCORE"));
	guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "0"));
	bloom::graphics::TextStyle style;
	style.blendingMode = bloom::graphics::TextStyle::BlendingMode::normal;
	style.foregroundColor = { 255, 255, 255, 0 };
	for (auto& st : guiElems)
		st->setStyle(style);

	ghostMovement.game = gameInstance;
	edibleSystem.game = gameInstance;
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

	// Render lives counter
	for (int i = 0; i < m_registry.get<Pacman>(playerEntity->getEntityID()).lives; ++i) {
		SDL_Rect srcRect = { 5 * 13,0,13,13 };
		SDL_Rect destRect = { (2 + i * 2) * TILESIZE,31 * TILESIZE + 2,13,13 };
		auto texPath = m_texturePath / "Entity" / "Pacman.png";
		m_gameInstance->textures.load(texPath)->render(srcRect, destRect);
	}

	SDL_SetRenderTarget(m_renderer, nullptr);
	SDL_RenderCopyEx(m_renderer, m_levelTex, nullptr, &GAMEAREA, 0.0, nullptr, SDL_FLIP_NONE);
	SDL_RenderCopyEx(m_renderer, m_entityLayer, nullptr, &GAMEAREA, 0.0, nullptr, SDL_FLIP_NONE);

	// Render GUI Text
	guiElems[0]->render(std::nullopt, SDL_Point{ 7 * TILESIZE - guiElems[0]->getTextWidth(), 1 * TILESIZE });
	guiElems[0]->render(std::nullopt, SDL_Point{ 17 * TILESIZE - guiElems[0]->getTextWidth(), 1 * TILESIZE });
	guiElems[1]->render(std::nullopt, SDL_Point{ 28 * TILESIZE - guiElems[1]->getTextWidth(), 0 });
	guiElems[2]->render(std::nullopt, SDL_Point{ 6 * TILESIZE - guiElems[2]->getTextWidth(), 0 });
	guiElems[3]->render(std::nullopt, SDL_Point{ 14 * TILESIZE - (guiElems[3]->getTextWidth() / 2), 0 });
}

void Level::changeLevel(const std::filesystem::path & levelFile, int levelNumber, const std::filesystem::path & texturePath) {
	m_levelFile = levelFile;
	m_levelNumber = levelNumber;
	m_texturePath = texturePath;
	m_pellets.clear();
	m_ghosts.clear();
	m_load(levelFile);
	m_generateEntities(layout, false);
	m_generatePellets(layout, false);
	m_generateTexture();
	m_cleanup();

	playerMovement.layout = layout;
	ghostMovement.layout = layout;
	speedDirector.layout = layout;
	gameDirector.levelNumber = levelNumber;

	FruitType fruit;
	{
		std::ifstream fin(levelFile.parent_path() / "LevelFruits.txt");
		int n;
		fin >> n;
		for (int i = 0; i < n; ++i) {
			int score;
			fin >> score;
			fruit = static_cast<FruitType>(score);
			if (i == levelNumber)
				break;
		}
	}
	guiElems[4]->setText(std::to_string(static_cast<int>(fruit)));
	gameDirector.setParameters(texturePath, fruit);
	gameDirector.init();
	speedDirector.init();

	animSysTest.update(0);
}

void Level::respawn() {
	m_ghosts.clear();
	m_generateEntities(m_originalLayout, true);
	refreshTexture();
}

void Level::m_load(const std::filesystem::path & levelData) {
	std::ifstream fin(levelData.u8string());

	int w, h;
	fin >> w >> h;

	layout = std::vector<std::vector<int>>(h, std::vector<int>(w));

	for (int i = 0; i < h; ++i)
		for (int j = 0; j < w; ++j)
			fin >> std::hex >> layout[i][j];

	m_originalLayout = layout;
}

void Level::m_generateTexture() {
	std::vector<FruitType> fruitIndicator;
	{
		std::ifstream fin(m_levelFile.parent_path() / "LevelFruits.txt");
		int a;
		fin >> a;
		std::vector<int> fruits(a);
		for (int i = 0; i < a; ++i)
			fin >> fruits[i];

		if (m_levelNumber < 7) {
			for (int i = 0; i < m_levelNumber + 1; ++i)
				fruitIndicator.emplace_back(static_cast<FruitType>(fruits[i]));
		}
		else if (m_levelNumber + 1 > 7 && m_levelNumber + 1 < 19) {
			for (int i = m_levelNumber - 6; i < m_levelNumber; ++i)
				fruitIndicator.emplace_back(static_cast<FruitType>(fruits[i]));
		}
		else if (m_levelNumber + 1 > 18) {
			for (int i = 0; i < 7; ++i)
				fruitIndicator.emplace_back(static_cast<FruitType>(fruits.back()));
		}
	}

	SDL_SetRenderTarget(m_renderer, m_levelTex);
	SDL_RenderClear(m_renderer);
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			if (layout[i][j] > 0) {
				std::filesystem::path tile = m_texturePath / "Tile" / std::string{ std::to_string(layout[i][j]) + ".png" };
				SDL_Rect src{ 0,0,TILETEXTURESIZE,TILETEXTURESIZE };
				SDL_Rect dest{ j * TILESIZE, i * TILESIZE,TILESIZE,TILESIZE };
				m_gameInstance->textures.load(tile)->render(src, dest);
			}

	for (int i = 0; i < fruitIndicator.size(); ++i) {
		SDL_Rect destRect = { (24 - i * 2) * TILESIZE,31 * TILESIZE + 2 };
		auto texPath = m_texturePath / "Entity";

		switch (fruitIndicator[i]) {
		default:
		case FruitType::cherry:
			texPath /= "Cherry.png";
			destRect.w = 12; destRect.h = 12;
			break;
		case FruitType::peach:
			texPath /= "Peach.png";
			destRect.w = 12, destRect.h = 12;
			break;
		case FruitType::apple:
			texPath /= "Apple.png";
			destRect.w = 12, destRect.h = 12;
			break;
		case FruitType::strawberry:
			texPath /= "Strawberry.png";
			destRect.w = 11, destRect.h = 12;
			break;
		case FruitType::galaxian:
			texPath /= "Galaxian.png";
			destRect.w = 11, destRect.h = 11;
			break;
		case FruitType::bell:
			texPath /= "Bell.png";
			destRect.w = 12, destRect.h = 13;
			break;
		case FruitType::grape:
			texPath /= "Grape.png";
			destRect.w = 11, destRect.h = 13;
			break;
		case FruitType::key:
			texPath /= "Key.png";
			destRect.w = 7, destRect.h = 13;
			break;
		}
		m_gameInstance->textures.load(texPath)->render(std::nullopt, destRect);
	}
	SDL_SetRenderTarget(m_renderer, nullptr);
}

void Level::m_generatePellets(std::vector<std::vector<int>> & layout, bool readOnly) {
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && ((std::abs(layout[i][j]) & 512) == 512)) {
				auto pellet = std::make_shared<PelletObject>(m_registry, m_gameInstance);
				pellet->init(m_texturePath / "Entity" / "Pellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_pellets.emplace_back(pellet);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 512);
			}if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1024) == 1024) {
				auto pellet = std::make_shared<PelletObject>(m_registry, m_gameInstance);
				pellet->init(m_texturePath / "Entity" / "PowerPellet.png", SDL_Rect{ j * TILESIZE,i * TILESIZE, TILESIZE,TILESIZE });
				m_pellets.emplace_back(pellet);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 1024);
			}
		}
}

void Level::m_generateEntities(std::vector<std::vector<int>> & layout, bool readOnly) {
	auto path = m_levelFile.parent_path();
	GhostInitDetails ghostDet;
	std::ifstream fin(path / "TimerData.txt");
	int entries;
	fin >> entries;
	int l, r;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 8; ++j)
			fin >> ghostDet.modeTimes[j];

		if (m_levelNumber >= l && m_levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(path / "PacmanSpeed.txt");
	fin >> entries;
	std::array<double, 2> pacmanSpeeds;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 2; ++j)
			fin >> pacmanSpeeds[j];

		if (m_levelNumber >= l && m_levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(path / "AfraidFlash.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		fin >> ghostDet.flashAmount;

		if (m_levelNumber >= l && m_levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(path / "FrightTimes.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> ghostDet.afraidTime;
		if (i == m_levelNumber)
			break;
	}

	fin = std::ifstream(path / "GhostSpeed.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		for (int j = 0; j < 3; ++j)
			fin >> ghostDet.multipliers[j];

		if (m_levelNumber >= l && m_levelNumber < r || r == -1)
			break;
	}

	fin = std::ifstream(path / "ElroySpeed.txt");
	fin >> entries;
	for (int i = 0; i < entries; ++i) {
		fin >> l >> r;
		std::pair<int, double> tmp;
		for (int j = 0; j < 2; ++j) {
			fin >> tmp.first >> tmp.second;
			ghostDet.elroyMultipliers[j] = tmp;
		}

		if (m_levelNumber >= l && m_levelNumber < r || r == -1)
			break;
	}

	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j) {
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 1) == 1) {
				if (playerEntity)
					playerEntity->init(m_texturePath / "Entity" / "Pacman.png", Tile{ j,i }, pacmanSpeeds);
				else {
					auto player = std::make_shared<Player>(m_registry, m_gameInstance);
					player->init(m_texturePath / "Entity" / "Pacman.png", Tile{ j,i }, pacmanSpeeds);
					playerEntity = player;
				}
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 1);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 2) == 2) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(m_texturePath / "Entity", Ghosts::shadow, Tile{ j,i }, ghostDet);
				m_ghosts.emplace_back(ghost);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 2);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 4) == 4) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				ghost->init(m_texturePath / "Entity", Ghosts::speedy, Tile{ j,i }, ghostDet);
				m_ghosts.emplace_back(ghost);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 4);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 8) == 8) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				auto newDet = ghostDet;
				if (m_levelNumber == 0)
					newDet.dotLimit = 30;
				ghost->init(m_texturePath / "Entity", Ghosts::bashful, Tile{ j,i }, newDet);
				m_ghosts.emplace_back(ghost);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 8);
			}
			if (layout[i][j] < 0 && (std::abs(layout[i][j]) & 16) == 16) {
				auto ghost = std::make_shared<GhostObject>(m_registry, m_gameInstance);
				auto newDet = ghostDet;
				if (m_levelNumber == 0)
					newDet.dotLimit = 90;
				else if (m_levelNumber == 1)
					newDet.dotLimit = 80;
				ghost->init(m_texturePath / "Entity", Ghosts::pokey, Tile{ j,i }, newDet);
				m_ghosts.emplace_back(ghost);
				if (!readOnly) layout[i][j] = -(-layout[i][j] | 16);
			}
		}
}

void Level::m_cleanup() {
	for (int i = 0; i < layout.size(); ++i)
		for (int j = 0; j < layout[i].size(); ++j)
			layout[i][j] = (layout[i][j] < 0 && (std::abs(layout[i][j]) & 256) != 256) ? 0 : layout[i][j];
}