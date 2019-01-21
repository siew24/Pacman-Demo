#pragma once
#include <algorithm>
#include "entt/entt.hpp"
#include "../Components/GhostComponent.h"
#include "../Components/PacmanComponent.h"
#include "Components/Position.h"
#include "../Configs.h"

using namespace bloom::components;
namespace ghostBehaviors {
	auto generateCandidates(const Tile& target, const Tile& currentTile, Ghost& ghost, std::vector<std::vector<int>>& layout, bool ComeOutOfTheHouse = true) {
		std::vector<std::pair<Direction, double>> posibilities{};
		if (Tile candidate{ currentTile.x, currentTile.y - 1 }; (layout[candidate.y][candidate.x] == 0 && ghost.direction != down) || (layout[candidate.y][candidate.x] == 39 && ComeOutOfTheHouse)) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(up, distance));
		}
		if (Tile candidate{ currentTile.x, currentTile.y + 1 }; layout[candidate.y][candidate.x] == 0 && ghost.direction != up) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(down, distance));
		}
		if (Tile candidate{ currentTile.x - 1, currentTile.y }; (currentTile.x == 0 || layout[candidate.y][candidate.x] == 0) && ghost.direction != right) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(left , distance));
		}
		if (Tile candidate{ currentTile.x + 1, currentTile.y }; (currentTile.x >= 27 || layout[candidate.y][candidate.x] == 0) && ghost.direction != left) {
			int xDist = std::abs(candidate.x - target.x), yDist = std::abs(candidate.y - target.y);
			double distance = std::sqrt(std::pow(xDist, 2) + std::pow(yDist, 2));
			posibilities.emplace_back(std::make_pair(right, distance));
		}

		std::sort(posibilities.begin(), posibilities.end(), [](auto& lhs, auto& rhs) {return lhs.second < rhs.second; });
		return posibilities;
	}
}