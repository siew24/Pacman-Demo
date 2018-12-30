#pragma once

class Pellet {
	using Position = bloom::components::Position;
	public:
		Pellet(Position a) : PelletPosition(a) {}

		Position PelletPosition;
};