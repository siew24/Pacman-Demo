#pragma once
#include <array>
#include "../Direction.h"
#include "../Tile.h"
#include "../Configs.h"

struct Pacman {
	Direction direction= Direction::null;
	Direction nextDir= Direction::null;

	int score = 0;
	int pelletsEaten = 0;
	int ghostsEaten = 0;					// Resets after a energizer (PowerPellet) is eaten
	std::array<double, 2> multipliers;

	bool dead = false;
	Direction lastDir;

	double timeAvailable = 0.0;
	double speed = 11.0 * TILESIZE;
	double currSpeed = speed;
};