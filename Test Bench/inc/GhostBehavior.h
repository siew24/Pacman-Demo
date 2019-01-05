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
		std::vector<Tile> posibilities{};
		Tile target{0,0};
		if (ghost.currentMode == chase)
			target = Tile{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
		else if (ghost.currentMode == scatter)
			target = Tile{ 25 ,-2 }; // Need coordinates to corner



		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };

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
		std::vector<Tile> posibilities{};
		Tile target{ 2,-2 };
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
			target = Tile{ 0,0 }; // Need coordinates to corner



		Tile currentTile{ (position.x + (TILESIZE / 2)) / TILESIZE, (position.y + (TILESIZE / 2)) / TILESIZE };

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
	/*
	Tile bashful(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>> layout) {

	}

	Tile pokey(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>> layout) {

	}*/
}