#pragma once

#include <functional>
#include "Components/Position.h"
#include "../Components/PacmanComponent.h"
#include "../Tile.h"
#include "../Configs.h"

using namespace bloom::components;

enum class Ghosts {
	shadow,
	speedy,
	bashful,
	pokey
};
enum class BehaviourModes {
	chase,
	scatter,
	afraid,
	dead
};


struct Ghost {
	std::function<Direction(entt::DefaultRegistry&, std::vector<std::vector<int>>&)> behavior;
	Tile spawnPoint;
	std::array<double, 8> modeTimes;
	int flashes =6;
	int dotLimit = 0;
	
	bool released = false;
	bool inHouse = true;

	BehaviourModes previousMode = BehaviourModes::scatter;
	BehaviourModes currentMode = BehaviourModes::scatter;

	double afraidTimer = 6.0;
	int modeLooped = 0;

	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;	

	double timeAvailable = 0.0;
	double speed = 11 * TILESIZE;
	double currSpeed = speed;
	Direction direction = Direction::left;
};