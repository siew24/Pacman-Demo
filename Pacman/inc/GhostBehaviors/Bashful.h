#pragma once

#include "Pathfinding.h"

namespace ghostBehaviors {
	Direction bashful(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout, Tile& currentTile) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"bashful"_hs>>()[0];
		entt::DefaultRegistry::entity_type shadowID = registry.view<entt::label<"shadow"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);
		auto& shadowPos = registry.get<Position>(ghostID);

		Tile shadowTile{ (shadowPos.x + (ENTITYSIZE / 2)) / TILESIZE,(shadowPos.y + (ENTITYSIZE / 2)) / TILESIZE };
		Tile target{ 2,-2 };

		if (ghost.currentMode == BehaviourModes::chase) {
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
			target.x += target.x - shadowTile.x;
			target.y += target.x - shadowTile.y;
		}
		else if (ghost.currentMode == BehaviourModes::scatter)
			target = Tile{ 27,31 }; // Need coordinates to corner
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