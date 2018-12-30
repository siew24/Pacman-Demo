#pragma once

#include "Framework.h"

struct Direction
{
	Direction(bloom::KeyboardKey k)
	{
		switch (k) {
		case bloom::KeyboardKey::KEY_W: {
				direction = "up";
				break;
		}
		case bloom::KeyboardKey::KEY_A: {
				direction = "left";
				break;
		}
		case bloom::KeyboardKey::KEY_S: {
				direction = "down";
				break;
		}
		case bloom::KeyboardKey::KEY_D: {
				direction = "right";
				break;
		}
		default:
				break;
		}
	}
	std::string direction = "";
};