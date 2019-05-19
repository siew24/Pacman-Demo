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

				while (potentialDistance > 0) {
					Tile currentTile = { (position.x + ENTITYSIZE / 2) / TILESIZE,(position.y + ENTITYSIZE / 2) / TILESIZE };
					if (((position.x + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0 && (position.y + (GHOST_TEXTURESIZE - TILESIZE) / 2) % TILESIZE == 0) || ghost.direction == Direction::null) {
						if (currentTile.x > 0 && currentTile.x <= 27 && currentTile.y > 0 && currentTile.y <= 30)
							ghost.direction = ghost.behavior(m_registry, layout);
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