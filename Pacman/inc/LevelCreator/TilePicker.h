#pragma once
#include <string>
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../Tile.h"

class TilePicker {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	TilePicker(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance), m_renderer(gameInstance->getRenderer()) {
		if (m_tileTex)
			SDL_DestroyTexture(m_tileTex);

		m_tileTex = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28 * TILESIZE), (33 * TILESIZE));

		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "Select a tile."));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "[ARROW KEYS] to select"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "[Enter] to confirm"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "[Escape] to cancel"));
		SDL_SetRenderTarget(m_renderer, m_tileTex);
		SDL_RenderClear(m_renderer);
		int x = 0;
		int y = 0;
		for (int i = 0; i < 40; ++i) {
			SDL_Rect tile{ x,y,16,16 };
			m_gameInstance->textures.load(ASSETPATH / "Assets" / "Tile" / (std::to_string(i) + ".png"))->render(std::nullopt, tile);

			x += 32;
			if (x > 216) x = 0, y += 32;
		}
		SDL_SetRenderTarget(m_renderer, nullptr);
	}
	void draw() {
		SDL_RenderClear(m_renderer);
		SDL_RenderCopyEx(m_renderer, m_tileTex, nullptr, &GAMEAREA, 0.0, nullptr, SDL_FLIP_NONE);

		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 8, 224 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 8, 240 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 8, 256 });

		int x = GAMEAREA.x - 1;
		int y = GAMEAREA.y - 1;
		for (int i = 0; i < 40; ++i) {
			SDL_Rect tile{ x,y,3,3 };
			if (i == m_selectedTileID) {
				m_gameInstance->textures.load(ASSETPATH / "Assets" / "LevelCreator" / "TL Corner.png")->render(std::nullopt, tile);
				tile.x += 15;
				m_gameInstance->textures.load(ASSETPATH / "Assets" / "LevelCreator" / "TR Corner.png")->render(std::nullopt, tile);
				tile.y += 15;
				m_gameInstance->textures.load(ASSETPATH / "Assets" / "LevelCreator" / "BR Corner.png")->render(std::nullopt, tile);
				tile.x -= 15;
				m_gameInstance->textures.load(ASSETPATH / "Assets" / "LevelCreator" / "BL Corner.png")->render(std::nullopt, tile);
				break;
			}
			x += 32;
			if (x > 216) x = GAMEAREA.x - 1, y += 32;
		}
	}

	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;
		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			m_selectedTileID -= 7;
			if (m_selectedTileID < 0)
				m_selectedTileID = 39;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			m_selectedTileID += 7;
			if (m_selectedTileID > 39)
				m_selectedTileID = 0;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_LEFT)) {
			--m_selectedTileID;
			if (m_selectedTileID < 0)
				m_selectedTileID = 39;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RIGHT)) {
			++m_selectedTileID;
			if (m_selectedTileID > 39)
				m_selectedTileID = 0;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			selectionOpen = false;
			m_currentSelectedTileID = m_selectedTileID;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_ESCAPE)) {
			selectionOpen = false;
			m_selectedTileID = m_currentSelectedTileID;
			cancelled = true;
		}
	}
	int openSelector() {
		selectionOpen = true;
		cancelled = false;
		while (selectionOpen) {
			m_gameInstance->handleEvents();
			if (!m_gameInstance->isRunning()) break;
			m_gameInstance->clear();
			update();
			draw();
			m_gameInstance->render();
		}
		m_gameInstance->timer.restart();
		return m_currentSelectedTileID;
	}

	~TilePicker() {}

	bool cancelled = false;

private:
	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_tileTex = nullptr;

	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;

	Tile m_currentTile{ 0,0 };
	int m_selectedTileID = 1;
	int m_currentSelectedTileID = 1;
	bool selectionOpen = false;
};