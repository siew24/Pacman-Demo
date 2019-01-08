#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class PelletSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman& pac) { player = entity; });

		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);


		m_registry.view<Pellet, Position>().each(
			[&](auto entity, Pellet& pellet, Position& pelletPos) {
				if (((playerPos.x + (TILESIZE / 2)) / TILESIZE == pelletPos.x / TILESIZE) && ((playerPos.y + (TILESIZE / 2)) / TILESIZE == pelletPos.y / TILESIZE)) {
					playerState.score += pellet.points;
					++playerState.pelletsEaten;
					m_registry.destroy(entity);
				}
			}
		);

		m_registry.view<PowerPellet, Position>().each(
			[&](auto entity, PowerPellet& pellet, Position& pelletPos) {
				if (((playerPos.x + (TILESIZE / 2)) / TILESIZE == pelletPos.x / TILESIZE) && ((playerPos.y + (TILESIZE / 2)) / TILESIZE == pelletPos.y / TILESIZE)) {
					playerState.score += pellet.points;
					++playerState.pelletsEaten;
					m_registry.destroy(entity);

					m_registry.view<Ghost>().each(
						[](auto entity, Ghost& ghost) {
							if (ghost.currentMode != dead) {
								ghost.lastTile = Tile{ 0,0 };
								ghost.currentMode = afraid;
								ghost.modeTimer = 10.0;
							}
						}
					);

				}
			}
		);
	}
};