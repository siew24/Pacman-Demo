#pragma once

class Wall {
	using Position = bloom::components::Position;
	public:
		Wall(Position a) : WallPosition(a) {}

		Position WallPosition;
};