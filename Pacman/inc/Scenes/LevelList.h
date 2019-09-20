#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class LevelList {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	LevelList(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont, std::vector<std::filesystem::path> levels) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Highscores"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Filename"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Page 0"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "< Prev"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Next >"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Back"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "No custom levels."));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Try to create one. :)"));

		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Entry"));
		m_levels = levels;
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		

		int offset = 0;

		if (m_levels.empty()) {
			guiElems[6]->render(std::nullopt, SDL_Point{ 120 - (guiElems[6]->getTextWidth() / 2), 40 });
			guiElems[7]->render(std::nullopt, SDL_Point{ 120 - (guiElems[7]->getTextWidth() / 2), 48 });
		}
		else {
			guiElems[1]->render( std::nullopt, SDL_Point{ 8, 24 } );
			int endRange = pageNumber * 10 + 10;
			if (endRange > m_levels.size())
				endRange = m_levels.size();
			for (int i = pageNumber * 10; i < endRange; ++i) {
				if (i == currentSelection + (pageNumber * 10))
					guiElems[8]->setStyle(highlighted);
				else
					guiElems[8]->setStyle(bloom::graphics::TextStyle());

				guiElems[8]->setText(m_levels[i].filename().string());
				guiElems[8]->render(std::nullopt, SDL_Point{ 16, 40 + ((i - pageNumber * 10) * 8) + offset });

				offset += 8;
			}
			if (m_levels.size() > 10) {
				if (prev)
					guiElems[3]->render(std::nullopt, SDL_Point{ 16, 27 * 8 });
				guiElems[2]->render(std::nullopt, SDL_Point{ 120 - (guiElems[2]->getTextWidth() / 2), 27 * 8 });
				if (next)
					guiElems[4]->render(std::nullopt, SDL_Point{ 28 * TILESIZE - guiElems[4]->getTextWidth(), 27 * 8 });
			}
		}
		guiElems[5]->render(std::nullopt, SDL_Point{ 16, 30 * 8 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (m_levels.size() > 10) {
			prev = pageNumber > 0 ? true : false;
			next = (pageNumber * 10 < m_levels.size() - 10) ? true : false;
			int pages = std::ceil(m_levels.size() / 10.0);
			guiElems[2]->setText("Page " + std::to_string(pageNumber + 1) + "/" + std::to_string(pages));
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			if (currentSelection >= 0 && currentSelection < 10) {
				selected = 1;
				selectedPath = m_levels[currentSelection + pageNumber * 10];
			}
			else if (currentSelection == 10)
				--pageNumber;
			else if (currentSelection == 11)
				++pageNumber;
			else if (currentSelection == 12)
				selected = 0;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_ESCAPE)) {
			selected = 0;
		}
		if ( m_levels.empty() ) {
			currentSelection = 12;
		}
		else {
			if ( keyboard.wasDown( KeyboardKeys::KEY_LEFT ) && currentSelection == 11 ) {
				currentSelection = 10;
			}
			if ( keyboard.wasDown( KeyboardKeys::KEY_RIGHT ) && currentSelection == 10 ) {
				currentSelection = 11;
			}

			if ( keyboard.wasDown( KeyboardKeys::KEY_DOWN ) ) {
				if ( currentSelection == 9 ) {
					if ( prev )
						currentSelection = 10;
					else if ( next )
						currentSelection = 11;
					else
						currentSelection = 12;
				}
				else if ( currentSelection == 11 || currentSelection == 10 ) {
					currentSelection = 12;
				}
				else if ( currentSelection == 12 )
					currentSelection = 0;
				else {
					++currentSelection;
					if ( currentSelection + pageNumber * 10 >= m_levels.size() ) {
						if ( prev )
							currentSelection = 10;
						else if ( next )
							currentSelection = 11;
						else
							currentSelection = 12;
					}
				}
			}
			if ( keyboard.wasDown( KeyboardKeys::KEY_UP ) ) {
				if ( currentSelection == 12 ) {
					if ( prev )
						currentSelection = 10;
					else if ( next )
						currentSelection = 11;
					else {
						currentSelection = 9;

						if ( currentSelection + pageNumber * 10 >= m_levels.size() )
							currentSelection = m_levels.size() - 1 - pageNumber * 10;
					}
				}
				else if ( currentSelection == 11 || currentSelection == 10 ) {
					currentSelection = 9;
					if ( currentSelection + pageNumber * 10 >= m_levels.size() )
						currentSelection = m_levels.size() - 1 - pageNumber * 10;
				}
				else if ( currentSelection == 0 )
					currentSelection = 12;
				else
					--currentSelection;
			}
			if ( currentSelection == 11 && !next )
				currentSelection = 10;
			else if ( currentSelection == 10 && !prev )
				currentSelection = 11;
		}
		for (int i = 3; i < 6; ++i) {
			if (currentSelection == i+7)
				guiElems[i]->setStyle(highlighted);
			else
				guiElems[i]->setStyle(bloom::graphics::TextStyle());
		}
	}

	int selected = -1;
	std::filesystem::path selectedPath;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	std::vector<std::filesystem::path> m_levels;
	bloom::Game*& m_gameInstance;
	int currentSelection = 0;
	int pageNumber = 0;
	bool prev = false;
	bool next = false;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
	const bloom::graphics::TextStyle yellow{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{255,255,0,0} };
};