#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class FilenameDialog {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	FilenameDialog(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Enter a file name"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "file.txt"));

		// guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Submit"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Cancel"));
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		//guiElems[1]->render(std::nullopt, SDL_Point{ 132, 88 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 16, 88 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 16, 104 });
		//guiElems[4]->render(std::nullopt, SDL_Point{ 16, 120 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			--currentSelection;
			if (currentSelection < 1)
				currentSelection = 2;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			++currentSelection;
			if (currentSelection > 2)
				currentSelection = 1;
		}

		if (currentSelection == 1) {
			std::string tmp = keyboard.getPrintable();
			bool bs = false;
			for (char c : tmp) {
				if (c == '\b') {
					if (!fileName.empty() && bs == true) {
						fileName.pop_back();
						bs = false;
					}
					bs = true;
				}
				else if (c > 32 && c < 127)
					if (fileName.size() < 7)
						fileName += tmp;
			}
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			switch (currentSelection) {
			case 1:
				selected = 1;
				break;
			case 2:
				selected = -1;
				break;
			}
		}

		if (currentSelection == 1)
			guiElems[1]->setText(fileName + (fileName.size() < 7 ? "_" : ""));
		else
			guiElems[1]->setText((fileName.empty() ? "file.txt" : fileName));

		for (int i = 1; i < guiElems.size(); ++i) {
			if (i == currentSelection)
				guiElems[i]->setStyle(highlighted);
			else
				guiElems[i]->setStyle(bloom::graphics::TextStyle{});
		}
	}

	std::string openDialog() {
		selected = 0;
		while (selected == 0) {
			m_gameInstance->handleEvents();
			if (!m_gameInstance->isRunning()) break;
			m_gameInstance->clear();
			update();
			draw();
			m_gameInstance->render();
		}
		if (selected == 1) {
			return fileName;
		}
		return "";
	}


	int selected = -1;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	bloom::Game*& m_gameInstance;
	int currentSelection = 1;
	std::string fileName = "";

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};