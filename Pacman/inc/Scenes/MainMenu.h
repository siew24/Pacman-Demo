#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"

class MainMenu {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	MainMenu(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Start"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Level Editor"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "High Scores"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Settings"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "About"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Quit"));
		m_gameInstance->textures.load(ASSETPATH / "Assets" / "Logo.png");
	}

	void draw() {
		m_gameInstance->textures.find(ASSETPATH / "Assets" / "Logo.png")->render(std::nullopt, SDL_Rect{ 8 + 0, 8 + 0, 224, 39 });
		int offset = 0;
		for (int i = 0; i < guiElems.size(); ++i) {
			guiElems[i]->render(std::nullopt, SDL_Point{ 8 + 8, 8 + (10 + i) * 8 + offset });
			offset += 8;
		}
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;
		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			--currentSelection;
			if (currentSelection < 0)
				currentSelection += guiElems.size();
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_ESCAPE))
			currentSelection = 5;

		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			++currentSelection;
			if (currentSelection >= guiElems.size())
				currentSelection -= guiElems.size();
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			selected = currentSelection;
		}

		for (int i = 0; i < guiElems.size(); ++i) {
			if (i == currentSelection)
				guiElems[i]->setStyle(highlighted);
			else
				guiElems[i]->setStyle(bloom::graphics::TextStyle{});
		}
	}

	int selected = -1;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	bloom::Game*& m_gameInstance;
	int currentSelection = 0;


	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};