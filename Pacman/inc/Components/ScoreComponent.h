#pragma once
#include "Graphics/Sprite.h"
#include "../Direction.h"
#include "Components/Position.h"
#include "Components/Size.h"

struct ScoreComponent {
	Position oldPosition;
	Size oldSize;

	double currentTime = 0.0;
	double lifespan = 1.0;
};