#pragma once

#include "Framework.h"
#include "CollisionSystem.h"

class Wall : protected CollisionSystem
{
	public:

	protected:
		bool isWall = true;
		SDL_Rect Wall_Horizontal{ 0, 0, 0, 0};
		SDL_Rect Wall_Vertical{ 0, 0, 0, 0};
		SDL_Rect Wall_UpperRight{ 0, 0, 0, 0};
		SDL_Rect Wall_UpperLeft{ 0, 0, 0, 0};
		SDL_Rect Wall_LowerRight{ 0, 0, 0, 0};
		SDL_Rect Wall_LowerLeft{ 0, 0, 0, 0};
		SDL_Rect Wall_UpperLeft{ 0, 0, 0, 0};
};