#pragma once
#include <algorithm>
#include "entt/entt.hpp"
#include "../Components/GhostComponent.h"
#include "../Components/PacmanComponent.h"
#include "Components/Position.h"
#include "../Configs.h"



using namespace bloom::components;
namespace ghostBehaviors {
	bool valid(Tile tile, std::vector<std::vector<int>>& layout) {
		tile.x = tile.x < 0 ? layout[0].size() + tile.x : tile.x;
		tile.x = tile.x >= layout[0].size() ? tile.x % layout[0].size() : tile.x;

		return layout[tile.y][tile.x] == 0 || (std::abs(layout[tile.y][tile.x]) & 256) == 256;
	}


	auto generateCandidates(const Tile& target, const Tile& currentTile, Ghost& ghost, std::vector<std::vector<int>>& layout) {
		std::vector<std::pair<Direction, double>> posibilities{};
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; valid(candidate, layout) && (std::abs(layout[currentTile.y][currentTile.x]) & 256) != 256 && ghost.direction != Direction::down) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(Direction::up, distance));
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; valid(candidate, layout) && ghost.direction != Direction::up) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(Direction::down, distance));
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (currentTile.x == 0 || valid(candidate, layout)) && ghost.direction != Direction::right) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(Direction::left, distance));
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (currentTile.x >= 27 || valid(candidate, layout)) && ghost.direction != Direction::left) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(Direction::right, distance));
		}

		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; (candidate.y != 0) && ((layout[candidate.y][candidate.x] == 39 || layout[candidate.y - 1][candidate.x] == 39) && ghost.released && (ghost.currentMode == BehaviourModes::chase || ghost.currentMode == BehaviourModes::scatter))) {
			posibilities.clear();
			posibilities.emplace_back(std::make_pair(Direction::up, 0.0));
			ghost.inHouse = false;
		}

		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; (candidate.y != layout.size() - 1) && ((layout[candidate.y][candidate.x] == 39 || layout[candidate.y - 1][candidate.x] == 39) && ghost.currentMode == BehaviourModes::dead)) {
			posibilities.clear();
			posibilities.emplace_back(std::make_pair(Direction::down, 0.0));
			ghost.inHouse = true;
		}

		std::sort(posibilities.begin(), posibilities.end(), [](auto & lhs, auto & rhs) {
			return (lhs.second < rhs.second) // Highest priority if distance to target is shorter
				|| (lhs.second == rhs.second && lhs.first < rhs.first); // Higher priority based on the direction enums if equal distance
			});
		return posibilities;
	}
}