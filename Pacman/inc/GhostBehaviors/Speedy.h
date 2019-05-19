#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Direction speedy(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout, Tile& currentTile) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"speedy"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);


		Tile target{ 0,0 };
	reevaluate:
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
			if (currentTile == ghost.spawnPoint) {
				ghost.currentMode = BehaviourModes::chase;
				goto reevaluate;
			}
			else
				target = ghost.spawnPoint;

		auto posibilities = generateCandidates(target, currentTile, ghost, layout);

		if (posibilities.empty())
			return Direction::null;
		else if (ghost.currentMode == BehaviourModes::afraid)
			return posibilities[rand() % posibilities.size()].first;
		else
			return posibilities[0].first;
	}

}