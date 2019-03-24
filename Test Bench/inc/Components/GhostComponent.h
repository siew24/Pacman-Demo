#pragma once

#include <array>
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

struct GhostInitDetails {
	std::array<double, 8> modeTimes{ 10,10,10,10,10,10,10 };
	int flashAmount = 5;
	double afraidTime = 6.0;
	std::array<double, 3> multipliers{ 1.0,1.0,1.0 };
};



struct Ghost {
	std::function<Direction(entt::DefaultRegistry&, std::vector<std::vector<int>>&)> behavior;
	Tile spawnPoint;
	GhostInitDetails levelVars;

	int dotLimit = 0;
	
	bool released = false;
	bool inHouse = true;

	BehaviourModes previousMode = BehaviourModes::scatter;
	BehaviourModes currentMode = BehaviourModes::scatter;

	int modeLooped = 0;

	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;	
	double afraidTimer = 0;
	double timeAvailable = 0.0;
	double speed = 11 * TILESIZE;
	double currSpeed = speed;
	Direction direction = Direction::left;
};