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
				// Measure how much the ghost can move, to ensure accuracy
				int potentialDistance = 0;
				ghost.timeAvailable += (deltaTime.value() / 1000);
				potentialDistance = static_cast<int>(ghost.timeAvailable * ghost.currSpeed);
				ghost.timeAvailable -= potentialDistance / ghost.currSpeed;

				// Change modes
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
				if (ghost.currentMode == BehaviourModes::afraid && ghost.afraidTimer <= 0.0)
					ghost.currentMode = ghost.previousMode;


				// Move ghost
				while (potentialDistance > 0) {
					Tile currentTile = { (position.x + (GHOST_TEXTURESIZE - TILESIZE) / 2) / TILESIZE,(position.y + (GHOST_TEXTURESIZE - TILESIZE) / 2) / TILESIZE };
					if ((position.x + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0 && (position.y + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0) {
						if ((currentTile.x < 28 && currentTile.x >= 1) && layout[currentTile.y - 1][currentTile.x] == 39)
							ghost.inHouse = true;
						else if ((currentTile.x < 28 && currentTile.x >= 0) && layout[currentTile.y + 1][currentTile.x] == 39)
							ghost.inHouse = false;

						Tile predictTile = currentTile;
						ghost.direction = ghost.nextDir;
						switch (ghost.direction) {
						case Direction::left:
							--predictTile.x;
							break;
						case Direction::right:
							++predictTile.x;
							break;
						case Direction::up:
							--predictTile.y;
							break;
						case Direction::down:
							++predictTile.y;
							break;
						}

						if (predictTile.x > 0 && predictTile.x <= 27 && predictTile.y > 0 && predictTile.y <= 30)
							ghost.nextDir = ghost.behavior(m_registry, layout, predictTile);
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
			}
		);
	};
	std::vector<std::vector<int>> layout;
	double lastUpdate = 0;
	bloom::Game * game;
};