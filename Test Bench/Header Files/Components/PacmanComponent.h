#pragma once
#include "../Direction.h"

struct Pacman {
	Direction direction=null;
	Direction nextDir=null;
	int moveX=0, moveY=0;
};