#pragma once

#include <functional>
#include "Components/Position.h"
#include "../Components/PacmanComponent.h"

using namespace bloom::components;
struct Tile {
	int x, y;

	bool operator==(Tile& rhs) {
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(Tile& rhs) {
		return (x != rhs.x || y != rhs.y);
	}
};

enum Ghosts {
	red,
	pinky,
	blue,
	orange
};


struct Ghost {
	std::function<Tile(Ghost&, Position&, Pacman&, Position&, std::vector<std::vector<int>>&)> behavior;

	double afraidTimer = 0.0;

	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;
};