#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Direction speedy(entt::registry& registry, std::array<std::array<int, 31>, 28>& tilemap, std::array<std::array<int, 31>, 28>& specialMap, Tile& currentTile) {
		entt::registry::entity_type playerID = registry.view<Pacman>()[0];
		entt::registry::entity_type ghostID = registry.view<entt::tag<"speedy"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);


		Tile target{ 0,0 };

		if (ghost.currentMode == BehaviourModes::chase)
			switch (pac.lastDir) {
			case Direction::up:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE - 4 };
				break;
			case Direction::down:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE + 4 };
				break;
			case Direction::left:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE - 4, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE };
				break;
			case Direction::right:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE + 4, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE };
				break;
			}
		else if (ghost.currentMode == BehaviourModes::scatter)
			target = Tile{ 2,-2 }; // Need coordinates to corner
		else if (ghost.currentMode == BehaviourModes::dead)
			target = ghost.levelVars.homeTile;

		auto posibilities = generateCandidates(target, currentTile, ghost, tilemap, specialMap);

		if (posibilities.empty())
			return Direction::null;
		else if (ghost.currentMode == BehaviourModes::afraid)
			return posibilities[rand() % posibilities.size()].first;
		else
			return posibilities[0].first;
	}

}