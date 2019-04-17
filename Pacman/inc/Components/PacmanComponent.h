#pragma once
#include <array>
#include "../Direction.h"
#include "../Tile.h"
#include "../Configs.h"

struct Pacman {
	Direction direction = Direction::null;
	Direction nextDir = Direction::null;

	int score = 0;
	int pelletsEaten = 0;
	int ghostsEaten = 0;					// Resets after a energizer (PowerPellet) is eaten
	std::array<double, 2> multipliers;

	bool dead = false;
	Direction lastDir;

	double timeAvailable = 0.0;
	double speed = 11.0 * TILESIZE;
	double currSpeed = speed;

	bool died = false;
	double timeFromLastPellet;
	int lives = 2;

	void addScore(int add) {
		score += add;
		for (int i = 0; i < ((score - prevScore) / 10000); ++i) {
			lives += (lives < 5) ? 1 : 0;
			prevScore += 10000;
		}
	}
	int prevScore = 0;
};