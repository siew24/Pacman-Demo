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
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "GAME OVER"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, std::to_string(score)));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Name"));

		// guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Submit"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Don't submit"));
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 132, 88 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 16, 88 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 16, 104 });
		//guiElems[4]->render(std::nullopt, SDL_Point{ 16, 120 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			--currentSelection;
			if (currentSelection < 2)
				currentSelection = 3;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			++currentSelection;
			if (currentSelection > 3)
				currentSelection = 2;
		}

		if (currentSelection == 2) {
			std::string tmp = keyboard.getPrintable();
			bool bs = false;
			for (char c : tmp) {
				if (c == '\b') {
					if (!playerName.empty() && bs == true) {
						playerName.pop_back();
						bs = false;
					}
					bs = true;
				}
				else if (c > 32 && c < 127)
					if (playerName.size() < 12)
						playerName += tmp;
			}
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			switch (currentSelection) {
			case 2:
				selected = LeaderboardsStore::addEntry((playerName.empty() ? "ANON-KUN" : playerName), score);
				break;
			case 3:
				selected = -2;
				break;
			}
		}

		if (currentSelection == 2)
			guiElems[2]->setText(playerName + (playerName.size() < 12 ? "_" : ""));
		else
			guiElems[2]->setText((playerName.empty() ? "Name" : playerName));

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
	std::string playerName;
	const int score;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};