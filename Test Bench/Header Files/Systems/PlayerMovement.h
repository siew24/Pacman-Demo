#pragma once
#include "Framework.h"
#include "../Components/ComponentIncludes.h"

bool valid(int tile) {
	return (tile < 1 || tile > 36);
}

class PlayerMovement : public bloom::systems::System {
	using Position = bloom::components::Position;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		m_registry.view<Pacman, Position>().each(
			[&](auto entity, Pacman& player, Position& position) {
				if (deltaTime.value() < 500.0)
					lastUpdate += deltaTime.value() / 1000;
				const double speed = 11.0;
				const double unitTime = 11.0 / 1000.0;
				int potentialDistance = 0;
				if (lastUpdate > unitTime) {
					potentialDistance = static_cast<int>(lastUpdate / unitTime);
					lastUpdate = std::fmod(lastUpdate, unitTime);
				}


				while (potentialDistance > 0) {
					bool moved = 0;
					if (position.x % 8 == 0 && position.y % 8 == 0 && player.moveX == 0 && player.moveY == 0) {
						player.direction = player.nextDir;
						auto& AnimSet = m_registry.get<AnimationSet>(entity);
						switch (player.direction) {
						case right:
							AnimSet.changeAnimation("right");
							if (position.x / 8 >= 27)
								player.moveX = +16;
							else if (valid(layout[position.y / 8][(position.x / 8) + 1]))
								player.moveX += 8;
							break;
						case left:
							AnimSet.changeAnimation("left");
							if (position.x / 8 == 0)
								player.moveX = -16;
							else if (valid(layout[position.y / 8][(position.x / 8) - 1]))
								player.moveX -= 8;
							break;
						case up:
							AnimSet.changeAnimation("up");
							if (valid(layout[(position.y / 8) - 1][position.x / 8]))
								player.moveY -= 8;
							break;
						case down:
							AnimSet.changeAnimation("down");
							if (valid(layout[(position.y / 8) + 1][position.x / 8]))
								player.moveY += 8;
							break;
						}
					}

					if (player.moveX > 0) {
						position.x += 1;
						if (position.x == 28 * 8) {
							position.x = -8;
						}
						--potentialDistance;
						--player.moveX;
						moved = true;
					}
					else if (player.moveX < 0) {
						position.x -= 1;
						if (position.x == -8) {
							position.x = 28 * 8;
						}
						--potentialDistance;
						++player.moveX;
						moved = true;
					}
					else if (player.moveY < 0) {
						position.y -= 1;
						--potentialDistance;
						++player.moveY;
						moved = true;
					}
					else if (player.moveY > 0) {
						position.y += 1;
						--potentialDistance;
						--player.moveY;
						moved = true;
					}

					if (!moved)
						break;
				}
			}
		);
	};

	std::vector<std::vector<int>> layout;
	double lastUpdate = 0;
};