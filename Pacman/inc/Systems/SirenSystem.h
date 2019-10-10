#pragma once
#include <string_view>
#include "Systems/DefaultSystem.h"
#include "Input/InputManager.h"
#include "../Components/GhostComponent.h"

namespace ghostBehaviors {
	Direction shadow(entt::registry& registry, std::vector<std::vector<int>>& layout, Tile& currentTile);
}

class SirenSystem : public bloom::systems::System {
	using bloom::systems::System::DefaultSystem;

public:
	void update(double deltaTime = 0) override {
		bool afraid = false;
		bool dead = false;
		soundbyte = 7;
		m_registry.view<Ghost>().each([&](auto entity, Ghost& ghost) {
			if (ghost.behavior == ghostBehaviors::shadow) {
				if (ghost.currSpeed == ghost.speed * ghost.levelVars.elroyMultipliers[0].second)
					soundbyte = 8;
				else if (ghost.currSpeed == ghost.speed * ghost.levelVars.elroyMultipliers[1].second)
					soundbyte = 9;
			}

			if (ghost.currentMode == BehaviourModes::afraid)
				afraid = true;
			else if (ghost.currentMode == BehaviourModes::dead)
				dead = true;
			}
		);

		if (afraid)
			soundbyte = 10;
		if (dead)
			soundbyte = 11;
	}

	int soundbyte = 7;
};