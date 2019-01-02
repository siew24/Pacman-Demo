#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

bool valid(int tile) {
	return (tile == 0);
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
					lastUpdate += (deltaTime.value() / 1000)*speed;
				
				int potentialDistance = 0;
				if (lastUpdate > 1.0) {
					potentialDistance = static_cast<int>(lastUpdate/1.0);
					lastUpdate = std::fmod(lastUpdate, 1.0);
				}


				while (potentialDistance > 0) {
					bool moved = 0;
					if (position.x % TILESIZE == 0 && position.y % TILESIZE == 0 && player.moveX == 0 && player.moveY == 0) {
						player.direction = player.nextDir;
						auto& AnimSet = m_registry.get<AnimationSet>(entity);
						switch (player.direction) {
						case right:
							AnimSet.changeAnimation("right");
							if (position.x / TILESIZE >= 27)
								player.moveX = +TILESIZE*2;
							else if (valid(layout[position.y / TILESIZE][(position.x / TILESIZE) + 1]))
								player.moveX += TILESIZE;
							break;
						case left:
							AnimSet.changeAnimation("left");
							if (position.x / TILESIZE == 0)
								player.moveX = -TILESIZE*2;
							else if (valid(layout[position.y / TILESIZE][(position.x / TILESIZE) - 1]))
								player.moveX -= TILESIZE;
							break;
						case up:
							AnimSet.changeAnimation("up");
							if (valid(layout[(position.y / TILESIZE) - 1][position.x / TILESIZE]))
								player.moveY -= TILESIZE;
							break;
						case down:
							AnimSet.changeAnimation("down");
							if (valid(layout[(position.y / TILESIZE) + 1][position.x / TILESIZE]))
								player.moveY += TILESIZE;
							break;
						}
					}

					if (player.moveX > 0) {
						position.x += 1;
						if (position.x == 28 * TILESIZE) {
							position.x = -TILESIZE;
						}
						--potentialDistance;
						--player.moveX;
						moved = true;
					}
					else if (player.moveX < 0) {
						position.x -= 1;
						if (position.x == -TILESIZE) {
							position.x = 28 * TILESIZE;
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
	const double speed = 11 * TILESIZE;
};