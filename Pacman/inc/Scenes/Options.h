#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class Options {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	Options(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Options"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Ghost volume"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "  60  "));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Pacman volume"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "  60  "));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Music volume"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "  60  "));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->getRenderer(), guiFont, "Back"));

		m_gameInstance->textures.load(ASSETPATH / "Assets" / "Logo.png");
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 16, 40 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 132, 40 });

		guiElems[3]->render(std::nullopt, SDL_Point{ 16, 56 });
		guiElems[4]->render(std::nullopt, SDL_Point{ 132, 56 });
		guiElems[5]->render(std::nullopt, SDL_Point{ 16, 72 });
		guiElems[6]->render(std::nullopt, SDL_Point{ 132, 72 });

		guiElems[7]->render(std::nullopt, SDL_Point{ 16, 30 * 8 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (keyboard.wasDown(KeyboardKeys::KEY_UP)) {
			currentSelection -= 2;
			if (currentSelection < 0)
				currentSelection = 7;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_DOWN)) {
			currentSelection += 2;
			if (currentSelection > 7)
				currentSelection = 1;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_LEFT)) {
			if (currentSelection == 1)
				ConfigStore::ghostVolume = (ConfigStore::ghostVolume - 5 < 0) ? 0 : ConfigStore::ghostVolume - 5;
			else if (currentSelection == 3)
				ConfigStore::pacmanVolume = (ConfigStore::pacmanVolume - 5 < 0) ? 0 : ConfigStore::pacmanVolume - 5;
			else if (currentSelection == 5)
				ConfigStore::musicVolume = (ConfigStore::musicVolume - 5 < 0) ? 0 : ConfigStore::musicVolume - 5;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RIGHT)) {
			if (currentSelection == 1)
				ConfigStore::ghostVolume = (ConfigStore::ghostVolume + 5 > 100) ? 100 : ConfigStore::ghostVolume + 5;
			else if (currentSelection == 3)
				ConfigStore::pacmanVolume = (ConfigStore::pacmanVolume + 5 > 100) ? 100 : ConfigStore::pacmanVolume + 5;
			else if (currentSelection == 5)
				ConfigStore::musicVolume = (ConfigStore::musicVolume + 5 > 100) ? 100 : ConfigStore::musicVolume + 5;
		}
		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER)) {
			if (currentSelection == 7)
				selected = 1;
		}

		for (int i = 1; i < 8; i += 2) {
			if (currentSelection == i) {
				guiElems[i]->setStyle(highlighted);
				if (i != 7) {
					guiElems[i + 1]->setStyle(highlighted);
					if (i == 1)
						guiElems[i + 1]->setText(((ConfigStore::ghostVolume == 0) ? "  " : "< ")
							+ std::to_string(static_cast<int>(ConfigStore::ghostVolume)) 
							+ ((ConfigStore::ghostVolume == 100 ) ? "  " : " >"));
					else if (i == 3)
						guiElems[i + 1]->setText(((ConfigStore::ghostVolume == 0) ? "  " : "< ")
							+ std::to_string(static_cast<int>(ConfigStore::pacmanVolume))
							+ ((ConfigStore::ghostVolume == 100) ? "  " : " >"));
					else if (i == 5)
						guiElems[i + 1]->setText(((ConfigStore::ghostVolume == 0) ? "  " : "< ")
							+ std::to_string(static_cast<int>(ConfigStore::musicVolume))
							+ ((ConfigStore::ghostVolume == 100) ? "  " : " >"));
				}
			}
			else {
				guiElems[i]->setStyle(bloom::graphics::TextStyle());
				if (i != 7) {
					guiElems[i + 1]->setStyle(bloom::graphics::TextStyle());
					if (i == 1)
						guiElems[i + 1]->setText("  " + std::to_string(static_cast<int>(ConfigStore::ghostVolume)) + "  ");
					else if (i == 3)
						guiElems[i + 1]->setText("  " + std::to_string(static_cast<int>(ConfigStore::pacmanVolume)) + "  ");
					else if (i == 5)
						guiElems[i + 1]->setText("  " + std::to_string(static_cast<int>(ConfigStore::musicVolume)) + "  ");
				}
			}
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