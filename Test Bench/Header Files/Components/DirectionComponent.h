#pragma once

#include "Framework.h"


enum Dir {
	up,
	down,
	left,
	right
};

struct Direction
{
	Direction(bloom::KeyboardKey k)
	{
		switch (k) {
		case bloom::KeyboardKey::KEY_W:{
				direction = up;
				break;
		}
		case bloom::KeyboardKey::KEY_A: {
				direction = left;
				break;
		}
		case bloom::KeyboardKey::KEY_S: {
				direction = down;
				break;
		}
		case bloom::KeyboardKey::KEY_D: {
				direction = right;
				break;
		}
		default:
				break;
		}
	}

	Dir direction;

};