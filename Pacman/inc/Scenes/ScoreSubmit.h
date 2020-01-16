#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class ScoreSubmit {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	ScoreSubmit(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont, int score) : m_gameInstance(gameInstance), score(score) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "GAME OVER"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, std::to_string(score)))->setStyle(highlighted);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Name"))->setStyle(highlighted);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "[ENTER] TO SAVE SCORE"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "[ESCAPE] TO CANCEL"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "GAME OVER"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, std::to_string(score)));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Name"));

		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Don't submit"));
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 132, 88 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 16, 88 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 16, 104 });
		guiElems[4]->render(std::nullopt, SDL_Point{ 16, 120 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;
		std::string tmp = keyboard.getPrintable();
		bool bs = false;
		for (char c : tmp) {
			if (c == '\b') {
				if (!SessionStore::currentName.empty() && bs == true) {
					SessionStore::currentName.pop_back();
					bs = false;
				}
				bs = true;
			}
			else if (c > 32 && c < 127 && c != ' ' && c != '\t')
				if (SessionStore::currentName.size() < 12)
					SessionStore::currentName += tmp;
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_ESCAPE)) {
			selected = -2;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			selected = LeaderboardsStore::addEntry((SessionStore::currentName.empty() ? "ANON-KUN" : SessionStore::currentName), score);
		}

		guiElems[2]->setText(SessionStore::currentName + (SessionStore::currentName.size() < 12 ? "_" : ""));

		for (int i = 2; i < guiElems.size(); ++i) {
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
	int currentSelection = 2;
	const int score;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};