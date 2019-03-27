#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

namespace ghostBehaviors {
	Direction shadow(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout);
}

class SpeedDirectorSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	void init() {
		m_registry.view<Pacman>().each([&](auto entity, auto & pac) { player = &pac; });
	}
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		
		bool isFrightened = false;
		m_registry.view<Position, Ghost>().each([&](auto entity, auto & position, auto & ghost) {
			Tile ghostTile{ (position.x + (ENTITYSIZE / 2)) / TILESIZE, (position.y + (ENTITYSIZE / 2)) / TILESIZE };

			if (checkTunnel(ghostTile))
				ghost.currSpeed = ghost.speed * ghost.levelVars.multipliers[1];
			else if (ghost.currentMode == BehaviourModes::afraid)			// Frightened
			{
				ghost.currSpeed = ghost.speed * ghost.levelVars.multipliers[2];
				isFrightened = true;
			}
			else if (ghost.behavior == ghostBehaviors::shadow) {

				if (244 - player->pelletsEaten == ghost.levelVars.elroyMultipliers[0].first)
					ghost.currSpeed = ghost.speed * ghost.levelVars.elroyMultipliers[0].second;
				else if (244 - player->pelletsEaten == ghost.levelVars.elroyMultipliers[1].first)
					ghost.currSpeed = ghost.speed * ghost.levelVars.elroyMultipliers[1].second;
				else
					ghost.currSpeed = ghost.speed * ghost.levelVars.multipliers[0];
			}
			else
				ghost.currSpeed = ghost.speed * ghost.levelVars.multipliers[0];
			}
		);
		player->currSpeed = isFrightened ? player->speed * player->multipliers[1] : player->speed * player->multipliers[0];
	}
	std::vector<std::vector<int>> layout;

private:
	bool checkTunnel(Tile currTile) {
		if (currTile.y == 14 && (currTile.x < 5 || currTile.x > 22))
			return true;
		return false;
	}

	Pacman* player;
};