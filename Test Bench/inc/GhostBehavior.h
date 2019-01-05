#pragma once
#include "Components/GhostComponent.h"
#include "Components/PacmanComponent.h"
#include "Components/Position.h"
#include "Configs.h"

using namespace bloom::components;
namespace behaviours {
	Tile shadow(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>>& layout) {
		Tile target{ (pacPos.x + (TILESIZE / 2)) / TILESIZE, (pacPos.y + (TILESIZE / 2)) / TILESIZE };
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
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (position.x / TILESIZE == 0 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (position.x / TILESIZE >= 27 || layout[candidate.y][candidate.x] == 0) && candidate != ghost.lastTile) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double newDistance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			if (newDistance < shortest || shortest == -1.0) {
				next = candidate;
				shortest = newDistance;
			}
		}
		if (shortest < 0.0) next = currentTile;
		return next;
	}
	/*
	Tile speedy(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>> layout) {

	}

	Tile bashful(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>> layout) {

	}

	Tile inky(Ghost& ghost, Position& position, Pacman& pac, Position& pacPos, std::vector<std::vector<int>> layout) {

	}*/
}