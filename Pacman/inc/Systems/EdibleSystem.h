#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"
#include "../ConfigStore.h"

class EdibleSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	void update(double deltaTime = 0) override {
		entt::registry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman& pac) { player = entity; });

		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);

		Tile playerTile = Tile{ (playerPos.x + (PACMAN_TEXTURESIZE / 2)) / TILESIZE ,(playerPos.y + (PACMAN_TEXTURESIZE / 2)) / TILESIZE };

		playerState.timeFromLastPellet += (deltaTime / 1000.0);
		// Regular Pellets
		m_registry.group<>(entt::get<Pellet, Position>, entt::exclude<Eaten>).each(
			[&](auto entity, Pellet& pellet, Position& pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					pelletEaten = true;
					playerState.addScore(pellet.points);
					++playerState.pelletsEaten;
					++playerState.totalPellets;
					playerState.timeAvailable -= 1000.0 / 60.0;
					playerState.timeFromLastPellet = 0.0;
					//m_registry.destroy(entity);
					m_registry.assign<Eaten>(entity);
				}
			}
		);

		// Power Pellets
		m_registry.group<>(entt::get<PowerPellet, Position>, entt::exclude<Eaten>).each(
			[&](auto entity, PowerPellet& pellet, Position& pelletPos) {
				if (playerTile.x == pelletPos.x / TILESIZE && playerTile.y == pelletPos.y / TILESIZE) {
					pelletEaten = true;
					playerState.addScore(pellet.points);
					++playerState.pelletsEaten;
					++playerState.totalPellets;
					playerState.ghostsEaten = 0;
					playerState.timeAvailable -= (1000.0 / 60.0) * 3;
					playerState.timeFromLastPellet = 0.0;
					//m_registry.destroy(entity);
					m_registry.assign<Eaten>(entity);

					m_registry.view<Ghost>().each(
						[](auto entity, Ghost& ghost) {
							if (ghost.currentMode != BehaviourModes::dead && !ghost.inHouse) {
								ghost.lastTile = Tile{ 0,0 };
								if (ghost.currentMode != BehaviourModes::afraid)
									ghost.previousMode = ghost.currentMode;
								ghost.currentMode = BehaviourModes::afraid;
								ghost.afraidTimer = ghost.levelVars.afraidTime;
								switch (ghost.direction) {
								case Direction::up:
									ghost.nextDir = Direction::down;
									break;
								case Direction::down:
									ghost.nextDir = Direction::up;
									break;
								case Direction::left:
									ghost.nextDir = Direction::right;
									break;
								case Direction::right:
									ghost.nextDir = Direction::left;
									break;
								}
							}
						}
					);
				}
			}
		);

		// Fruits
		m_registry.group<>(entt::get<Fruit, Position>, entt::exclude<Eaten>).each(
			[&](auto entity, Fruit& fruit, Position& fruitPos) {
				if (!m_registry.has<Eaten>(entity)) {
					if ((playerPos.x + PACMAN_TEXTURESIZE / 2) / TILESIZE == (fruitPos.x + fruit.rect.w / 2) / TILESIZE
						&& (playerPos.y + PACMAN_TEXTURESIZE / 2) / TILESIZE == (fruitPos.y + fruit.rect.h / 2) / TILESIZE) {
						bonusEaten = true;
						playerState.addScore(static_cast<int>(fruit.type));

						auto& size = m_registry.get<Size>(entity);
						fruitPos.x += size.w / 2 - BONUS_SIZE.w / 2, fruitPos.y += size.h / 2 - BONUS_SIZE.h / 2;
						size.w = BONUS_SIZE.w, size.h = BONUS_SIZE.h;
						m_registry.assign_or_replace<Sprite>(entity, game->textures.load(ASSETPATH / "Assets" / "Scores" / "Bonus" / (std::to_string(static_cast<int>(fruit.type)) + ".png")));
						m_registry.assign<ScoreComponent>(entity);
						m_registry.remove<Fruit>(entity);
					}
					else
					{
						if (fruit.timeSpawned / 1000 > 9 && fruit.timeSpawned / 1000 < 10)
							m_registry.assign<Eaten>(entity);//m_registry.destroy(entity);
						else
							fruit.timeSpawned += deltaTime;
					}
				}
			}
		);

		m_registry.group<>(entt::get<Ghost, Position>, entt::exclude<Eaten>).each(
			[&](auto entity, Ghost& ghost, Position& ghostPos) {
				if (Tile ghostT{ (ghostPos.x + (GHOST_TEXTURESIZE / 2)) / TILESIZE,(ghostPos.y + (GHOST_TEXTURESIZE / 2)) / TILESIZE }; playerTile == ghostT) {
					if (ghost.currentMode == BehaviourModes::afraid) {
						ghostEaten = true;
						int addScore = 200 * pow(2, playerState.ghostsEaten);
						playerState.score += addScore;
						++playerState.ghostsEaten;
						playerState.addScore((playerState.ghostsEaten >= 4) ? 12000 : 0);
						ghost.currentMode = BehaviourModes::dead;

						Size& size = m_registry.get<Size>(entity);
						ScoreComponent popup{ ghostPos, size };
						ghostPos.x += size.w / 2 - GHOST_BONUS_SIZE.w / 2; ghostPos.y += size.h / 2 - GHOST_BONUS_SIZE.h / 2;
						size.w = GHOST_BONUS_SIZE.w, size.h = GHOST_BONUS_SIZE.h;
						m_registry.assign_or_replace<Sprite>(entity, game->textures.load(ASSETPATH / "Assets" / "Scores" / "Ghost" / (std::to_string(addScore) + ".png")));
						m_registry.get<AnimationSet>(player).changeCurrent("blank");
						m_registry.get<AnimationPtr>(player)->update(1);
						m_registry.assign<ScoreComponent>(entity, popup);
					}
					else if (ghost.currentMode != BehaviourModes::dead && !playerState.godMode)
						playerState.dead = true, playerState.died = true, --playerState.lives;
				}
			}
		);
	}




	bloom::Game* game;

	bool pelletEaten = false;
	bool ghostEaten = false;
	bool bonusEaten = false;
};