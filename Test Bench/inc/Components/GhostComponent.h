#pragma once

#include <functional>
#include "Components/Position.h"
#include "../Components/PacmanComponent.h"
#include "../Tile.h"
#include "../Configs.h"

using namespace bloom::components;

enum Ghosts {
	shadow,
	speedy,
	bashful,
	pokey
};
enum BehaviourModes {
	chase,
	scatter,
	afraid,
	dead
};


struct Ghost {
	std::function<Direction(entt::DefaultRegistry&, std::vector<std::vector<int>>&)> behavior;
	Tile spawnPoint;
	int dotLimit = 0;
	bool released = false;
	bool inHouse = true;

	BehaviourModes previousMode = scatter;
	BehaviourModes currentMode = scatter;
	double modeTimer = 70.0;
	double afraidTimer = 60.0;
	int modeLooped = 0;						// Between scatter and chase

	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;	

	double timeAvailable = 0.0;
	double speed = 11 * TILESIZE;
	double currspeed = speed;
	Direction direction = left;


};