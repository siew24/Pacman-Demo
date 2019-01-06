#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Tile pokey(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"pokey"_hs>>()[0];
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);
		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };
		Tile target{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE };

		int xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
		double currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));

	reevaluate:
		if (currentDistance <= 8.0 && ghost.currentMode == chase || ghost.currentMode == scatter)
			target = Tile{ 0,31 }; // Need coordinates to corner
		else if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint) {
				ghost.currentMode = chase;
				goto reevaluate;
			}
			else
				target = ghost.spawnPoint;

		auto posibilities = generateCandidates(target, currentTile, ghost, layout);

		if (posibilities.empty())
			return currentTile;
		else if (ghost.currentMode == afraid)
			return posibilities[rand() % posibilities.size()].first;
		else
			return posibilities[0].first;
	}
}