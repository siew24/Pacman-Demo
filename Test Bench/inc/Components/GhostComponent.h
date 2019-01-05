#pragma once

#include <functional>
#include "Components/Position.h"
#include "../Components/PacmanComponent.h"
#include "../Tile.h"

using namespace bloom::components;

enum Ghosts {
	shadow,
	speedy,
	blue,
	orange
};
enum BehaviourModes {
	chase,
	scatter,
	afraid
};


struct Ghost {
	std::function<Tile(Ghost&, Position&, Pacman&, Position&, std::vector<std::vector<int>>&)> behavior;

	BehaviourModes currentMode = chase;
	double modeTimer = 9999999999.0;

	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;
};