#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class SpeedDirectorSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		m_registry.view<Position, Ghost>().each([&](auto entity, auto& position, auto& ghost) {
			Tile ghostTile{ (position.x + ENTITYSIZE / 2) / TILESIZE, (position.y + ENTITYSIZE / 2) / TILESIZE };

			if (Check_Tunnel(ghostTile))
				ghost.currspeed = ghost.speed * 0.4;
			else if (ghost.currentMode == 2)			// Frightened
			{
				ghost.currspeed = ghost.speed * 0.5;
				is_Frightened = true;
			}
			else
				ghost.currspeed = ghost.speed * 0.75;
		}
		);
		m_registry.view<Position, Pacman>().each([&](auto entity, auto& position, auto& pac) {
			Tile playerTile{ (position.x + ENTITYSIZE / 2) / TILESIZE, (position.y + ENTITYSIZE / 2) / TILESIZE };

			if (Check_Tunnel(playerTile))
				pac.currspeed = pac.speed * 0.5;
			else if (is_Frightened)						// Frightened Ghosts
			{
				pac.currspeed = pac.speed * 0.9;
				is_Frightened = false;
			}
			else
				pac.currspeed = pac.speed * 0.8;
		}
		);
			
	}

private:
	bool Check_Tunnel(Tile pos) {
		if (pos.y == 15 * TILESIZE && (pos.x > 0 && pos.x < 5 * TILESIZE) || (pos.x > 22 * TILESIZE && pos.x < 27 * TILESIZE))
			return true;
		return false;
	}
	bool is_Frightened = false;
};