#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Direction speedy(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"speedy"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);

		Tile currentTile{ (position.x + (ENTITYSIZE / 2)) / TILESIZE, (position.y + (ENTITYSIZE / 2)) / TILESIZE };

		Tile target{ 0,0 };
	reevaluate:
		if (ghost.currentMode == chase)
			switch (pac.lastDir) {
			case up:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE - 4 };
				break;
			case down:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE + 4 };
				break;
			case left:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE - 4, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE };
				break;
			case right:
				target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE + 4, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE };
				break;
			}
		else if (ghost.currentMode == scatter)
			target = Tile{ 2,-2 }; // Need coordinates to corner
		else if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint) {
				ghost.currentMode = chase;
				goto reevaluate;
			}
			else
				target = ghost.spawnPoint;

		auto posibilities = generateCandidates(target, currentTile, ghost, layout);

		if (posibilities.empty())
			return ghost.direction;
		else if (ghost.currentMode == afraid)
			return posibilities[rand() % posibilities.size()].first;
		else
			return posibilities[0].first;
	}

}