#pragma once
#include "Framework.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class PelletSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman& pac) {player = entity; });

		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerStat = m_registry.get<Pacman>(player);

		m_registry.view<Pellet, Position>().each(
			[&](auto entity, Pellet& pellet, Position& pelletPos) {
				if (((playerPos.x+(TILESIZE/2)) / TILESIZE == pelletPos.x / TILESIZE) && ((playerPos.y + (TILESIZE / 2)) / TILESIZE == pelletPos.y / TILESIZE)) {
					playerStat.score += pellet.points;
					m_registry.destroy(entity);
				}
			}
		);
		
	}
};