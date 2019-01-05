#pragma once
#include "Components/GhostComponent.h"
#include "Components/PacmanComponent.h"
#include "Components/Position.h"
#include "Configs.h"

using namespace bloom::components;
namespace behaviours {
	Tile shadow(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"shadow"_hs>>()[0];
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);
		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };

		std::vector<Tile> posibilities{};
		Tile target{ 0,0 };
		if (ghost.currentMode == chase)
			target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
		else if (ghost.currentMode == scatter)
			target = Tile{ 25 ,-2 }; // Need coordinates to corner
		else if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint)
				ghost.currentMode = chase;
			else
				target = ghost.spawnPoint;

		

		int xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
		double currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));


		Tile next = target;
		double shortest = -1.0;
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (position.x / TILESIZE == 0 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (position.x / TILESIZE >= 27 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (shortest < 0.0) next = currentTile;
		if (ghost.currentMode == afraid && !posibilities.empty())
			next = posibilities[rand() % posibilities.size()];
		return next;
	}

	Tile speedy(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"speedy"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);

		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };

		std::vector<Tile> posibilities{};
		Tile target{ 0,0 };
		if (ghost.currentMode == chase)
			switch (pac.lastDir) {
			case up:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE - 4 };
				break;
			case down:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE + 4 };
				break;
			case left:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE - 4, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
				break;
			case right:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE + 4, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
				break;
			}
		else if (ghost.currentMode == scatter)
			target = Tile{ 2,-2 }; // Need coordinates to corner
		else if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint)
				ghost.currentMode = chase;
			else
				target = ghost.spawnPoint;




		int xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
		double currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));


		Tile next = target;
		double shortest = -1.0;
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (position.x / TILESIZE == 0 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (position.x / TILESIZE >= 27 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (shortest < 0.0) next = currentTile;
		if (ghost.currentMode == afraid && !posibilities.empty())
			next = posibilities[rand() % posibilities.size()];
		return next;
	}

	Tile bashful(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"bashful"_hs>>()[0];
		entt::DefaultRegistry::entity_type shadowID = registry.view<entt::label<"shadow"_hs>>()[0];
		auto& pac = registry.get<Pacman>(playerID);
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);
		auto& shadowPos = registry.get<Position>(ghostID);
		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };

		Tile shadowTile{ (shadowPos.x + (TILESIZE / 2)) / TILESIZE,(shadowPos.y + (TILESIZE / 2)) / TILESIZE };


		std::vector<Tile> posibilities{};
		Tile target{ 2,-2 };
		if (ghost.currentMode == chase) {
			switch (pac.lastDir) {
			case up:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE - 4 };
				break;
			case down:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE + 4 };
				break;
			case left:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE - 4, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
				break;
			case right:
				target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE + 4, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
				break;
			}
			target.x += target.x - shadowTile.x;
			target.y += target.x - shadowTile.y;
		}
		else if (ghost.currentMode == scatter)
			target = Tile{ 27,31 }; // Need coordinates to corner
		else if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint)
				ghost.currentMode = chase;
			else
				target = ghost.spawnPoint;





		int xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
		double currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));


		Tile next = target;
		double shortest = -1.0;
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (position.x / TILESIZE == 0 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (position.x / TILESIZE >= 27 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (shortest < 0.0) next = currentTile;
		if (ghost.currentMode == afraid && !posibilities.empty())
			next = posibilities[rand() % posibilities.size()];
		return next;
	}
	
	Tile pokey(entt::DefaultRegistry& registry, std::vector<std::vector<int>>& layout) {
		entt::DefaultRegistry::entity_type playerID = registry.view<Pacman>()[0];
		entt::DefaultRegistry::entity_type ghostID = registry.view<entt::label<"pokey"_hs>>()[0];
		auto& pacPos = registry.get<Position>(playerID);
		auto& position = registry.get<Position>(ghostID);
		auto& ghost = registry.get<Ghost>(ghostID);
		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };
		std::vector<Tile> posibilities{};
		Tile target{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE };

	if (ghost.currentMode == dead)
			if (currentTile == ghost.spawnPoint)
				ghost.currentMode = chase;
			else
				target = ghost.spawnPoint;



		int xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
		double currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));

		
		if (currentDistance <= 8.0 && ghost.currentMode == chase || ghost.currentMode == scatter) {
			target = Tile{ 0,31 }; // Need coordinates to corner
			xDist = std::abs(currentTile.x - target.x), yDist = std::abs(currentTile.y - target.y);
			currentDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
		}


		Tile next = target;
		double shortest = -1.0;
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (position.x / TILESIZE == 0 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (position.x / TILESIZE >= 27 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
			posibilities.emplace_back(candidate);
		}
		if (shortest < 0.0) next = currentTile;
		if (ghost.currentMode == afraid && !posibilities.empty())
			next = posibilities[rand() % posibilities.size()];
		return next;
	}
}