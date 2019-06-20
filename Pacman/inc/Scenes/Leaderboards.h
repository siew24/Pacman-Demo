#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class Leaderboards {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	Leaderboards(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont, int scorePos) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Highscores"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Rank"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Name"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Score"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Page 0"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "< Prev"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Next >"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Back"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "No scores set."));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Be the first to do so. :)"));

		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Entry"));
		m_gameInstance->textures.load(ASSETPATH / "Assets" / "Logo.png");
		highlightedScore = scorePos;
		pageNumber = scorePos / 10;
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 8, 24 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 48, 24 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 160, 24 });

		int offset = 0;

		if (LeaderboardsStore::leaderboards.empty()) {
			guiElems[8]->render(std::nullopt, SDL_Point{ 120 - (guiElems[8]->getTextWidth() / 2), 40 });
			guiElems[9]->render(std::nullopt, SDL_Point{ 120 - (guiElems[9]->getTextWidth() / 2), 48 });
		}
		else {
			int endRange = pageNumber * 10 + 10;
			if (endRange > LeaderboardsStore::leaderboards.size())
				endRange = LeaderboardsStore::leaderboards.size();
			for (int i = pageNumber * 10; i < endRange; ++i) {
				if (i == highlightedScore)
					guiElems[10]->setStyle(yellow);
				else
					guiElems[10]->setStyle(bloom::graphics::TextStyle());

				guiElems[10]->setText(std::to_string(i + 1));
				guiElems[10]->render(std::nullopt, SDL_Point{ 16, 40 + ((i - pageNumber * 10) * 8) + offset });

				guiElems[10]->setText(LeaderboardsStore::leaderboards[i].first);
				guiElems[10]->render(std::nullopt, SDL_Point{ 48, 40 + ((i - pageNumber * 10) * 8) + offset });

				guiElems[10]->setText(std::to_string(LeaderboardsStore::leaderboards[i].second));
				guiElems[10]->render(std::nullopt, SDL_Point{ 160, 40 + ((i - pageNumber * 10) * 8) + offset });

				offset += 8;

				if (LeaderboardsStore::leaderboards.size() > 10) {
					if (prev)
						guiElems[5]->render(std::nullopt, SDL_Point{ 16, 27 * 8 });
					guiElems[4]->render(std::nullopt, SDL_Point{ 120 - (guiElems[4]->getTextWidth() / 2), 27 * 8 });
					if (next)
						guiElems[6]->render(std::nullopt, SDL_Point{ 28 * TILESIZE - guiElems[6]->getTextWidth(), 27 * 8 });
				}
			}
		}
		guiElems[7]->render(std::nullopt, SDL_Point{ 16, 30 * 8 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (LeaderboardsStore::leaderboards.size() > 10) {
			prev = pageNumber > 0 ? true : false;
			next = (pageNumber * 10 < LeaderboardsStore::leaderboards.size() - 10) ? true : false;
			int pages = std::ceil(LeaderboardsStore::leaderboards.size() / 10.0);
			guiElems[4]->setText("Page " + std::to_string(pageNumber + 1) + "/" + std::to_string(pages));
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			if (currentSelection == 0)
				--pageNumber;
			else if (currentSelection == 1)
				++pageNumber;
			else if (currentSelection == 2)
				selected = 1;
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_LEFT) && currentSelection == 1) {
			currentSelection = 0;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RIGHT) && currentSelection == 0) {
			currentSelection = 1;
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			if (currentSelection == 0 || currentSelection == 1)
				currentSelection = 2;
			else if (currentSelection == 2)
				currentSelection = 0;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			if (currentSelection == 0 || currentSelection == 1)
				currentSelection = 2;
			else if (currentSelection == 2)
				currentSelection = 0;
		}

		if (currentSelection == 1 && next == false)
			currentSelection = 0;
		if (currentSelection == 0 && prev == false)
			if (next)
				currentSelection = 1;
			else
				currentSelection = 2;

		for (int i = 0; i < 3; ++i) {
			if (currentSelection == i)
				guiElems[5 + i]->setStyle(highlighted);
			else
				guiElems[5 + i]->setStyle(bloom::graphics::TextStyle());
		}
	}

	int selected = -1;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	bloom::Game*& m_gameInstance;
	int currentSelection = 0;
	int pageNumber = 0;
	int highlightedScore;
	bool prev = false;
	bool next = false;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
	const bloom::graphics::TextStyle yellow{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{255,255,0,0} };
};