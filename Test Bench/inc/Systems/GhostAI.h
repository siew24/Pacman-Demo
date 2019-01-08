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
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman& pac) { player = entity; });
		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);

		m_registry.view<Ghost, Position>().each(
			[&](auto entity, Ghost& ghost, Position& position) {
				int potentialDistance = 0;
				if (deltaTime.value() < 500.0) {
					ghost.modeTimer -= (deltaTime.value()+0.5) / 1000;
					if (ghost.modeTimer <= 0.0) {
						if (ghost.currentMode == chase) {
							ghost.modeTimer = 10.0 - ghost.modeTimer; // Need timer
							ghost.currentMode = scatter;
						}
						else if (ghost.currentMode == dead)
							ghost.modeTimer = 0;
						else {
							ghost.modeTimer = 10.0 - ghost.modeTimer; // Need timer
							ghost.currentMode = chase;
						}
					}
					potentialDistance = static_cast<int>(((deltaTime.value()+0.5) / 1000)*speed);
				}

				
				

				bool moved = true;
				while (potentialDistance > 0 && moved) {
					moved = false;
					if (Tile currentTile{ position.x / TILESIZE,position.y / TILESIZE }; position.x % TILESIZE == 0 && position.y % TILESIZE == 0 && ghost.moveX == 0 && ghost.moveY == 0) {
						Tile nextTile = ghost.behavior(m_registry, layout);
						AnimationSet& animSet = m_registry.get<AnimationSet>(entity);
						if (nextTile.x > currentTile.x) {
							if (ghost.currentMode == afraid)
								animSet.changeAnimation("afraid");
							else if (ghost.currentMode == dead)
								animSet.changeAnimation("rightd");
							else
								animSet.changeAnimation("right");
							if (currentTile.x >= 27)
								ghost.moveX = +TILESIZE * 2;
							else
								ghost.moveX += TILESIZE;
						}
						else if (nextTile.x < currentTile.x) {
							if (ghost.currentMode == afraid)
								animSet.changeAnimation("afraid");
							else if (ghost.currentMode == dead)
								animSet.changeAnimation("leftd");
							else
								animSet.changeAnimation("left");
							if (currentTile.x == 0)
								ghost.moveX = -TILESIZE * 2;
							else
								ghost.moveX -= TILESIZE;
						}
						else if (nextTile.y > currentTile.y) {
							if (ghost.currentMode == afraid)
								animSet.changeAnimation("afraid");
							else if (ghost.currentMode == dead)
								animSet.changeAnimation("downd");
							else
								animSet.changeAnimation("down");
							ghost.moveY += TILESIZE;
						}
						else if (nextTile.y < currentTile.y) {
							if (ghost.currentMode == afraid)
								animSet.changeAnimation("afraid");
							else if (ghost.currentMode == dead)
								animSet.changeAnimation("upd");
							else
								animSet.changeAnimation("up");
							ghost.moveY -= TILESIZE;
						}
						ghost.lastTile = currentTile;
					}

					if (ghost.moveX > 0) {
						position.x += 1;
						if (position.x == 28 * TILESIZE) {
							position.x = -TILESIZE;
						}
						--potentialDistance;
						--ghost.moveX;
						moved = true;
					}
					else if (ghost.moveX < 0) {
						position.x -= 1;
						if (position.x == -TILESIZE) {
							position.x = 28 * TILESIZE;
						}
						--potentialDistance;
						++ghost.moveX;
						moved = true;
					}
					else if (ghost.moveY < 0) {
						position.y -= 1;
						--potentialDistance;
						++ghost.moveY;
						moved = true;
					}
					else if (ghost.moveY > 0) {
						position.y += 1;
						--potentialDistance;
						--ghost.moveY;
						moved = true;
					}

				}
				if (Tile ghostT{ (position.x + (TILESIZE / 2)) / TILESIZE,(position.y + (TILESIZE / 2)) / TILESIZE }, pac{ (playerPos.x + (TILESIZE / 2)) / TILESIZE,(playerPos.y + (TILESIZE / 2)) / TILESIZE }; pac == ghostT) {
					if (ghost.currentMode == afraid) {
						playerState.score += 6000;
						ghost.currentMode = dead;
					}
					else if (ghost.currentMode != dead)
						playerState.dead = true;
				}
			}
		);
	};
	std::vector<std::vector<int>> layout;
	double lastUpdate = 0;
	const double speed = 11 * TILESIZE;
};