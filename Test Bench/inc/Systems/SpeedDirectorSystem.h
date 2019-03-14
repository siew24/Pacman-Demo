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

			if (Check_Tunnel(position))
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

			if (Check_Tunnel(position))
				pac.currspeed = pac.speed * 0.4;
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
	std::vector<std::vector<int>> layout;

private:
	bool Check_Tunnel(Position pos) {
		if(pos.x >= 0 && pos.x <= 27)
			if (Tile currTile{ (pos.x + (ENTITYSIZE / 2)) / TILESIZE, (pos.y + (ENTITYSIZE / 2)) / TILESIZE };std::abs(layout[currTile.y][currTile.x]) == 12)
				return true;
		return false;
	}
	bool is_Frightened = false;
};