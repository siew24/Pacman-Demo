#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"

class SelectLevel {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	SelectLevel(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Choose a level set"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Official"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Custom"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Cancel"));
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		int offset = 0;
		for (int i = 1; i < guiElems.size(); ++i) {
			guiElems[i]->render(std::nullopt, SDL_Point{ 8 + 8, 8 + (10 + i) * 8 + offset });
			offset += 8;
		}
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;
		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			--currentSelection;
			if (currentSelection < 1)
				currentSelection = guiElems.size() - 1;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			++currentSelection;
			if (currentSelection >= guiElems.size())
				currentSelection = 1;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			selected = currentSelection;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_ESCAPE)) {
			selected = 3;
		}


		for (int i = 1; i < guiElems.size(); ++i) {
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
	int currentSelection = 1;


	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};