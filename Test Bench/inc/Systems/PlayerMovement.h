#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class PlayerMovement : public bloom::systems::System {
	using Position = bloom::components::Position;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		m_registry.view<Pacman, Position>().each(
			[&](auto entity, Pacman& player, Position& position) {
				int potentialDistance = 0;
				if (deltaTime.value() < 500.0) {
					auto timeAvailable = (deltaTime.value()+0.5) / 1000.0;
					player.penalty -= timeAvailable;
					if (player.penalty <= 0.0) {
						timeAvailable = -player.penalty;
						player.penalty = 0.0;
					}

					potentialDistance = static_cast<int>(timeAvailable*speed);
				}

				Tile playerTile{ (position.x + TILESIZE / 2) / TILESIZE, (position.y + TILESIZE / 2) / TILESIZE };

				if (playerTile.x < 27 || playerTile.x >= 0)
				{
					// Change anim
					auto& AnimSet = m_registry.get<AnimationSet>(entity);
					if (player.nextDir != player.direction) {
						player.direction = player.nextDir;
						switch (player.direction) {
						case right:
							AnimSet.changeAnimation("right");
							break;
						case left:
							AnimSet.changeAnimation("left");
							break;
						case up:
							AnimSet.changeAnimation("up");
							break;
						case down:
							AnimSet.changeAnimation("down");
							break;
						}
					}
					player.lastDir = player.direction != null ? player.direction : player.lastDir;
				}

				Tile nextTile = playerTile;
				{
					auto tmp = nextTile;
					switch (player.direction) {
					case up:
						tmp = { nextTile.x, nextTile.y - 1 };
						break;
					case down:
						tmp = { nextTile.x, nextTile.y + 1 };
						break;
					case left:
						tmp = { nextTile.x - 1, nextTile.y };
						break;
					case right:
						tmp = { nextTile.x + 1, nextTile.y };
						break;
					}
					if (valid(tmp))
						nextTile = tmp;
				}

				while (potentialDistance > 0) {
					bool moved = 0;
					if (nextTile.x * TILESIZE > position.x) {
						++position.x;
						moved = true;
					}
					else if (nextTile.x * TILESIZE < position.x) {
						--position.x; moved = true;
					}
					if (nextTile.y  * TILESIZE > position.y) {
						++position.y; moved = true;
					}
					else if (nextTile.y * TILESIZE < position.y) {
						--position.y; moved = true;
					}
					--potentialDistance;

					if (!moved)
						break;

					if (position.x == 28 * TILESIZE)
						position.x = -TILESIZE;
					else if (position.x == -TILESIZE)
						position.x = 28 * TILESIZE;
				}
			}
		);
	};

	std::vector<std::vector<int>> layout;
	const double speed = 11 * TILESIZE;

private:
	bool valid(Tile tile) {
		tile.x = tile.x < 0 ? layout[0].size() + tile.x : tile.x;
		tile.x = tile.x >= layout[0].size() ? tile.x % layout[0].size() : tile.x;

		return (layout[tile.y][tile.x] == 0);
	}
};