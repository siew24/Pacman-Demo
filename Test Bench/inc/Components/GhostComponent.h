#pragma once

#include <functional>
#include "Components/Position.h"
#include "../Components/PacmanComponent.h"
#include "../Tile.h"

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
	std::function<Tile(entt::DefaultRegistry&, std::vector<std::vector<int>>&)> behavior;
	Tile spawnPoint;
	BehaviourModes currentMode = chase;
	double modeTimer = 10.0;
	


	Tile lastTile = { 0,0 };
	int moveX = 0, moveY = 0;	

	double timeAvailable = 0.0;
	
};