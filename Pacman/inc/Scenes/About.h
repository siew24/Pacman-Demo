#pragma once
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../ConfigStore.h"

class About {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	About(bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont) : m_gameInstance(gameInstance) {
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "About"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "BF Pacman"))->setStyle(title);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Lead Programmer"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Derrick Timmermans"))->setStyle(grey);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Assisting Programmer"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Siew Ee Shin"))->setStyle(grey);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "BloomFramework"))->setStyle(title);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Programmers"));
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Anatoli Pitikin"))->setStyle(grey);
		guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_gameInstance->_getRenderer(), guiFont, "Back"))->setStyle(highlighted);

		m_gameInstance->textures.load(ASSETPATH / "Assets" / "Logo.png");
	}

	void draw() {
		guiElems[0]->render(std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 });
		guiElems[1]->render(std::nullopt, SDL_Point{ 120 - (guiElems[1]->getTextWidth() / 2), 56 });
		guiElems[2]->render(std::nullopt, SDL_Point{ 16, 72 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 32, 88 });
		guiElems[4]->render(std::nullopt, SDL_Point{ 16, 112 });
		guiElems[5]->render(std::nullopt, SDL_Point{ 32, 128 });
		guiElems[6]->render(std::nullopt, SDL_Point{ 120 - (guiElems[6]->getTextWidth() / 2), 168 });
		guiElems[7]->render(std::nullopt, SDL_Point{ 16,184 });
		guiElems[3]->render(std::nullopt, SDL_Point{ 32, 200 });
		guiElems[8]->render(std::nullopt, SDL_Point{ 32,208 });

		guiElems[9]->render(std::nullopt, SDL_Point{ 16,  240 });
	}
	void update() {
		auto& keyboard = m_gameInstance->input.keyboard;

		if (keyboard.wasDown(KeyboardKeys::KEY_RETURN) || keyboard.wasDown(KeyboardKeys::KEY_KEYPAD_ENTER))
			selected = 1;
	}

	int selected = -1;

private:
	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;
	bloom::Game*& m_gameInstance;

	const bloom::graphics::TextStyle highlighted{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{179,0,89,0} };
	const bloom::graphics::TextStyle grey{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{255,255,255,100} };
	const bloom::graphics::TextStyle title{ bloom::graphics::TextStyle::BlendingMode::normal, SDL_Color{255,255,0,0} };
};