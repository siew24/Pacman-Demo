#pragma once
#include "../Direction.h"

struct Pacman {
	Direction direction;
	Direction nextDir;
	int moveX=0, moveY=0;
};