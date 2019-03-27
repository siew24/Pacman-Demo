#pragma once

enum class FruitType {
	cherry = 100,
	strawberry = 300,
	peach = 500,
	apple = 700,
	grape = 1000,
	galaxian = 2000,
	bell = 3000,
	key = 5000
};
struct Fruit {
	double timeSpawned = 0.0;
	FruitType type = FruitType::cherry;
	SDL_Rect rect;
};