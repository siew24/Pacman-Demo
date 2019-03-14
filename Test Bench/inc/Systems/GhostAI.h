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
					ghost.timeAvailable += (deltaTime.value() / 1000);
					potentialDistance = static_cast<int>(ghost.timeAvailable*ghost.currspeed);
					ghost.timeAvailable -= potentialDistance / ghost.currspeed;
					ghost.modeTimer -= potentialDistance / ghost.currspeed;

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
				}
				AnimationSet& animSet = m_registry.get<AnimationSet>(entity);
				while (potentialDistance > 0) {
					Tile currentTile = { (position.x + ENTITYSIZE / 2) / TILESIZE,(position.y + ENTITYSIZE / 2) / TILESIZE };
					if (((position.x+2 ) % TILESIZE == 0 && (position.y+2) % TILESIZE == 0) || ghost.direction == null ) {
						if(currentTile.x > 0 && currentTile.x <= 27 && currentTile.y > 0 && currentTile.y <= 30)
						ghost.direction = ghost.behavior(m_registry, layout);

						std::string animset = "";
						switch (ghost.direction) {
						case left:
							animset += "left";
							break;
						case right:
							animset += "right";
							break;
						case up:
							animset += "up";
							break;
						case down:
							animset += "down";
							break;
						default:
							animset += "left";
							break;
						}
						switch (ghost.currentMode) {
						case dead:
							animset += "d";
							break;
						case afraid:
							animset = "afraid";
							break;
						}

						animSet.changeAnimation(animset);
					}
					switch (ghost.direction) {
					case left:
						position.x = (position.x - 1) == -TILESIZE ? 28 * TILESIZE: position.x-1;
						--potentialDistance;
						break;
					case right:
						position.x = (position.x + 1) == 28 * TILESIZE ? -TILESIZE : position.x + 1;
						--potentialDistance;
						break;
					case up:
						position.y = (position.y - 1) == -TILESIZE ? 31 * TILESIZE : position.y - 1;
						--potentialDistance;
						break;
					case down:
						position.y = (position.y + 1) == 31 * TILESIZE ? -TILESIZE : position.y + 1;
						--potentialDistance;
						break;
					default:
						--potentialDistance;
						break;
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
};