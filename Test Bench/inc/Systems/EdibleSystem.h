#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class EdibleSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman & pac) { player = entity; });

		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);

		Tile playerTile = Tile{ (playerPos.x + (PACMAN_TEXTURESIZE / 2)) / TILESIZE ,(playerPos.y + (PACMAN_TEXTURESIZE / 2)) / TILESIZE };

		// Regular Pellets
		m_registry.view<Pellet, Position>().each(
			[&](auto entity, Pellet & pellet, Position & pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					playerState.score += pellet.points;
					++playerState.pelletsEaten;
					playerState.timeAvailable -= 1000.0 / 60.0;
					m_registry.destroy(entity);
				}
			}
		);

		// Power Pellets
		m_registry.view<PowerPellet, Position>().each(
			[&](auto entity, PowerPellet & pellet, Position & pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					playerState.score += pellet.points;
					++playerState.pelletsEaten;
					playerState.timeAvailable -= (1000.0 / 60.0) * 3;
					m_registry.destroy(entity);

					m_registry.view<Ghost>().each(
						[](auto entity, Ghost & ghost) {
							if (ghost.currentMode != BehaviourModes::dead && !ghost.inHouse) {
								ghost.lastTile = Tile{ 0,0 };
								if (ghost.currentMode != BehaviourModes::afraid)
									ghost.previousMode = ghost.currentMode;
								ghost.currentMode = BehaviourModes::afraid;
								ghost.afraidTimer = ghost.levelVars.afraidTime;
							}
						}
					);
				}
			}
		);

		// Fruits
		m_registry.view<Fruit, Position>().each(
			[&](auto entity, Fruit & fruit, Position & fruitPos) {
				if ((playerPos.x + PACMAN_TEXTURESIZE / 2) / TILESIZE == (fruitPos.x + fruit.rect.w / 2) / TILESIZE
					&& (playerPos.y + PACMAN_TEXTURESIZE / 2) / TILESIZE == (fruitPos.y + fruit.rect.h / 2) / TILESIZE) {
					playerState.score += static_cast<int>(fruit.type);

					m_registry.destroy(entity);
				}
				else
				{
					if (fruit.timeSpawned / 1000 > 9 && fruit.timeSpawned / 1000 < 10)
						m_registry.destroy(entity);
					else
						fruit.timeSpawned += deltaTime.value();
				}
			}
		);
	}
};