#pragma once
#include <string_view>
#include "Systems/DefaultSystem.h"
#include "Input/InputManager.h"
#include "../Direction.h"
#include "../Components/PacmanComponent.h"

const std::string_view KONAMI{ "UUDDLRLRBA" };

class InputHandlerSystem : public bloom::systems::System {
	using bloom::systems::System::DefaultSystem;
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;
	using MouseButtons = bloom::input::MouseButton;

public:
	InputHandlerSystem(entt::registry& registry, InputManager& inputMngr) :DefaultSystem(registry), inputManager(inputMngr) {}

	void update(double deltaTime = 0) override {
		auto& keyboardState = inputManager.keyboard;
		auto registryView = m_registry.view<Pacman>();
		if (!registryView.empty()) {
			timePassed += deltaTime;
			Pacman& playerComponent = m_registry.get<Pacman>(registryView[0]);
			if (keyboardState.wasDown(KeyboardKeys::KEY_UP) || keyboardState.wasDown(KeyboardKeys::KEY_W))
				playerComponent.nextDir = Direction::up;
			else if (keyboardState.wasDown(KeyboardKeys::KEY_DOWN) || keyboardState.wasDown(KeyboardKeys::KEY_S))
				playerComponent.nextDir = Direction::down;
			else if (keyboardState.wasDown(KeyboardKeys::KEY_LEFT) || keyboardState.wasDown(KeyboardKeys::KEY_A))
				playerComponent.nextDir = Direction::left;
			else if (keyboardState.wasDown(KeyboardKeys::KEY_RIGHT) || keyboardState.wasDown(KeyboardKeys::KEY_D))
				playerComponent.nextDir = Direction::right;
			else if (keyboardState.wasDown(KeyboardKeys::KEY_ESCAPE))
				if (timePassed > 2000.0)
					if (ConfigStore::instaQuit)
						quit = true;
					else
						timePassed = 0;
				else
					quit = true;


			if (keyboardState.wasDown(KeyboardKeys::KEY_UP))
				buffer.push_back('U');
			else if (keyboardState.wasDown(KeyboardKeys::KEY_DOWN))
				buffer.push_back('D');
			else if (keyboardState.wasDown(KeyboardKeys::KEY_LEFT))
				buffer.push_back('L');
			else if (keyboardState.wasDown(KeyboardKeys::KEY_RIGHT))
				buffer.push_back('R');
			else if (keyboardState.wasDown(KeyboardKeys::KEY_A))
				buffer.push_back('A');
			else if (keyboardState.wasDown(KeyboardKeys::KEY_B))
				buffer.push_back('B');

			if (buffer == KONAMI) {
				playerComponent.godMode = (playerComponent.godMode) ? false : true;
				std::cout << ((playerComponent.godMode) ? "GODMODE ACTIVATED" : "GODMODE DEACTIVATED") << std::endl;
				buffer.clear();
			}
			else if (buffer != KONAMI.substr(0, buffer.size())) {
				buffer.clear();
			}
		}
	}

	double timePassed = 2000;
	bool quit = false;

private:
	std::string buffer{};
	InputManager& inputManager;
};