#pragma once
#include "../Direction.h"
#include "../Tile.h"

struct Pacman {
	
	Direction direction=null;
	Direction nextDir=null;
	int moveX=0, moveY=0;

	int score = 0;
	int pelletsEaten = 0;

	bool dead = false;
	Direction lastDir;

	double timeAvailable = 0.0;
};