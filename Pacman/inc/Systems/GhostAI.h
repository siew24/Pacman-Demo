#pragma once
#include "Systems/Systems.h"
#include "../Components/GhostComponent.h"
#include "../Components/PacmanComponent.h"
#include "../Configs.h"

class GhostAI : public bloom::systems::System {
	using Position = bloom::components::Position;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman & pac) { player = entity; });
		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);

		m_registry.view<Ghost, Position>().each(
			[&](auto entity, Ghost & ghost, Position & position) {
				int potentialDistance = 0;

				if (deltaTime.value() < 500.0) {
					ghost.timeAvailable += (deltaTime.value() / 1000);
					potentialDistance = static_cast<int>(ghost.timeAvailable * ghost.currSpeed);
					ghost.timeAvailable -= potentialDistance / ghost.currSpeed;
					if (ghost.currentMode == BehaviourModes::afraid)
						ghost.afraidTimer -= potentialDistance / ghost.currSpeed;
					else
						ghost.levelVars.modeTimes[ghost.modeLooped] -= potentialDistance / ghost.currSpeed;

					if (ghost.modeLooped < 8 && ghost.levelVars.modeTimes[ghost.modeLooped] <= 0.0) {
						if (ghost.currentMode == BehaviourModes::chase) {
							ghost.currentMode = BehaviourModes::scatter;
							++ghost.modeLooped;
						}
						else {
							ghost.currentMode = BehaviourModes::chase;
							++ghost.modeLooped;
						}
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

					if (ghost.currentMode == BehaviourModes::afraid && ghost.afraidTimer <= 0.0)
						ghost.currentMode = ghost.previousMode;
				}
				AnimationSet& animSet = m_registry.get<AnimationSet>(entity);
				while (potentialDistance > 0) {
					Tile currentTile = { (position.x + ENTITYSIZE / 2) / TILESIZE,(position.y + ENTITYSIZE / 2) / TILESIZE };
					if (((position.x + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0 && (position.y + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0) || ghost.direction == Direction::null) {
						if (currentTile.x > 0 && currentTile.x <= 27 && currentTile.y > 0 && currentTile.y <= 30)
							ghost.direction = ghost.behavior(m_registry, layout);

						std::string animset = "";
						switch (ghost.direction) {
						case Direction::left:
							animset += "left";
							break;
						case Direction::right:
							animset += "right";
							break;
						case Direction::up:
							animset += "up";
							break;
						case Direction::down:
							animset += "down";
							break;
						default:
							animset += "left";
							break;
						}
						switch (ghost.currentMode) {
						case BehaviourModes::dead:
							animset += "d";
							break;
						case BehaviourModes::afraid:
							animset = (ghost.afraidTimer <= ghost.levelVars.flashAmount * (ENTITYFRAMETIME / 1000.0) * 4) ? "afraidFlash" : "afraid";
							break;
						}

						animSet.changeCurrent(animset);
					}
					switch (ghost.direction) {
					case Direction::left:
						position.x = (position.x - 1) == -TILESIZE ? 28 * TILESIZE : position.x - 1;
						--potentialDistance;
						break;
					case Direction::right:
						position.x = (position.x + 1) == 28 * TILESIZE ? -TILESIZE : position.x + 1;
						--potentialDistance;
						break;
					case Direction::up:
						position.y = (position.y - 1) == -TILESIZE ? 31 * TILESIZE : position.y - 1;
						--potentialDistance;
						break;
					case Direction::down:
						position.y = (position.y + 1) == 31 * TILESIZE ? -TILESIZE : position.y + 1;
						--potentialDistance;
						break;
					default:
						--potentialDistance;
						break;
					}
				}
				if (Tile ghostT{ (position.x + (GHOST_TEXTURESIZE / 2)) / TILESIZE,(position.y + (GHOST_TEXTURESIZE / 2)) / TILESIZE }, pac{ (playerPos.x + (PACMAN_TEXTURESIZE / 2)) / TILESIZE,(playerPos.y + (PACMAN_TEXTURESIZE / 2)) / TILESIZE }; pac == ghostT) {
					if (ghost.currentMode == BehaviourModes::afraid) {
						int addScore = 200 * pow(2, playerState.ghostsEaten);
						playerState.score += addScore;
						++playerState.ghostsEaten;
						playerState.addScore((playerState.ghostsEaten >= 4) ? 12000 : 0);
						ghost.currentMode = BehaviourModes::dead;

						Size & size = m_registry.get<Size>(entity);
						ScoreComponent popup{ position, size };
						++position.x; position.y += GHOST_BONUS_SIZE.y;
						size.w = GHOST_BONUS_SIZE.w, size.h = GHOST_BONUS_SIZE.h;
						m_registry.accommodate<Sprite>(entity, game->textures.load(ASSETPATH / "Assets" / "Scores" / "Ghost" / (std::to_string(addScore) + ".png")));
						m_registry.assign<ScoreComponent>(entity, popup);
					}
					else if (ghost.currentMode != BehaviourModes::dead)
						playerState.dead = true, playerState.died = true, --playerState.lives;
				}
			}
		);
	};
	std::vector<std::vector<int>> layout;
	double lastUpdate = 0;
	bloom::Game* game;
};