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

		playerState.timeFromLastPellet += (deltaTime.value() / 1000.0);
		// Regular Pellets
		m_registry.view<Pellet, Position>().each(
			[&](auto entity, Pellet & pellet, Position & pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					playerState.addScore(pellet.points);
					++playerState.pelletsEaten;
					playerState.timeAvailable -= 1000.0 / 60.0;
					playerState.timeFromLastPellet = 0.0;
					m_registry.destroy(entity);
				}
			}
		);

		// Power Pellets
		m_registry.view<PowerPellet, Position>().each(
			[&](auto entity, PowerPellet & pellet, Position & pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					playerState.addScore(pellet.points);
					++playerState.pelletsEaten;
					playerState.ghostsEaten = 0;
					playerState.timeAvailable -= (1000.0 / 60.0) * 3;
					playerState.timeFromLastPellet = 0.0;
					m_registry.destroy(entity);

					m_registry.view<Ghost>().each(
						[](auto entity, Ghost & ghost) {
							if (ghost.currentMode != BehaviourModes::dead && !ghost.inHouse) {
								ghost.lastTile = Tile{ 0,0 };
								if (ghost.currentMode != BehaviourModes::afraid)
									ghost.previousMode = ghost.currentMode;
								ghost.currentMode = BehaviourModes::afraid;
								ghost.afraidTimer = ghost.levelVars.afraidTime;
								switch (ghost.direction) {
								case Direction::up:
									ghost.direction = Direction::down;
									break;
								case Direction::down:
									ghost.direction = Direction::up;
									break;
								case Direction::left:
									ghost.direction = Direction::right;
									break;
								case Direction::right:
									ghost.direction = Direction::left;
									break;
								}
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
					playerState.addScore(static_cast<int>(fruit.type));

					auto& size = m_registry.get<Size>(entity);
					fruitPos.x += size.w / 2 - BONUS_SIZE.w / 2, fruitPos.y += size.h / 2 - BONUS_SIZE.h / 2;
					size.w = BONUS_SIZE.w, size.h = BONUS_SIZE.h;
					m_registry.accommodate<Sprite>(entity, game->textures.load(ASSETPATH / "Assets" / "Scores" / "Bonus" / (std::to_string(static_cast<int>(fruit.type)) + ".png")));
					m_registry.assign<ScoreComponent>(entity);
					m_registry.remove<Fruit>(entity);
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

	bloom::Game* game;
};