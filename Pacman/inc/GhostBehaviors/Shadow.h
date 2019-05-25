#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Direction shadow(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout, Tile& currentTile) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"shadow"_hs>>()[0];
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);

		Tile target{ 0,0 };

		if (ghost.currentMode == BehaviourModes::chase)
			target = Tile{ (pacPos.x + (ENTITYSIZE / 2)) / TILESIZE, (pacPos.y + (ENTITYSIZE / 2)) / TILESIZE };
		else if (ghost.currentMode == BehaviourModes::scatter)
			target = Tile{ 25 ,-2 };
		else if (ghost.currentMode == BehaviourModes::dead)
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