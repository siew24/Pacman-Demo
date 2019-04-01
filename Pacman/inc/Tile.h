#pragma once
struct Tile {
	int x, y;

	bool operator==(Tile& rhs) {
		return (x == rhs.x && y == rhs.y);
	}

	bool operator!=(Tile& rhs) {
		return (x != rhs.x || y != rhs.y);
	}
};