#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "FilenameDialog.h"

class FileMenu {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	FileMenu(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance), dialog(gameInstance, guiFont) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "File Options"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Save As"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Open"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Cancel"));
		m_gameInstance->textures.load(ASSETPATH / "Assets" / "Logo.png");
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
				currentSelection = 3;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			++currentSelection;
			if (currentSelection > 3)
				currentSelection = 1;
		}

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			if (currentSelection == 3)
				selected = -1;
			else if (currentSelection == 2)
				selected = 2;
			else if (currentSelection == 1)
				selected = 1;
		}

		for (int i = 1; i < 4; ++i) {
			if (currentSelection == i) {
				guiElems[i]->setStyle(highlighted);
			}
			else {
				guiElems[i]->setStyle(bloom::graphics::TextStyle());
			}
		}
	}
	std::pair<std::string, std::string> openMenu() {
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
			return std::make_pair("save", dialog.openDialog());
		}else if(selected == 2)
			return std::make_pair("open", dialog.openDialog());
		return std::make_pair("", "");
	}

	int selected = -1;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	bloom::Game*& m_gameInstance;
	int currentSelection = 1;
	bool selectionOpen = false;
	FilenameDialog dialog;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
};