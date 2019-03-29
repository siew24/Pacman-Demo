#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class PlayerMovement : public bloom::systems::System {
	using Position = bloom::components::Position;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	void update(std::optional<double> deltaTime = std::nullopt) override {
		m_registry.view<Pacman, Position>().each(
			[&](auto entity, Pacman& player, Position& position) {
				int potentialDistance = 0;
				if (deltaTime.value() < 500.0) {
					player.timeAvailable += deltaTime.value() / 1000.0;

					potentialDistance = static_cast<int>(player.timeAvailable*player.currSpeed);
					player.timeAvailable -= potentialDistance / player.currSpeed;
				}

				Tile playerTile{ (position.x + PACMAN_TEXTURESIZE / 2) / TILESIZE, (position.y + PACMAN_TEXTURESIZE / 2) / TILESIZE };

				if (playerTile.x < 27 || playerTile.x >= 0)
				{
					// Change anim
					auto& AnimSet = m_registry.get<AnimationSet>(entity);
					if (player.nextDir != player.direction) {
						Tile nextTile = playerTile;
						{
							auto tmp = nextTile;
							switch (player.nextDir) {
							case Direction::up:
								tmp = { nextTile.x, nextTile.y - 1 };
								break;
							case Direction::down:
								tmp = { nextTile.x, nextTile.y + 1 };
								break;
							case Direction::left:
								tmp = { nextTile.x - 1, nextTile.y };
								break;
							case Direction::right:
								tmp = { nextTile.x + 1, nextTile.y };
								break;
							}
							if (valid(tmp)) {
								nextTile = tmp;
								player.direction = player.nextDir;
								switch (player.direction) {
								case Direction::right:
									AnimSet.changeAnimation("right");
									break;
								case Direction::left:
									AnimSet.changeAnimation("left");
									break;
								case Direction::up:
									AnimSet.changeAnimation("up");
									break;
								case Direction::down:
									AnimSet.changeAnimation("down");
									break;
								}
							}
						}
					}
					player.lastDir = player.direction != Direction::null ? player.direction : player.lastDir;
				}


				if (player.direction != Direction::null && potentialDistance > 0) {
					Tile nextTile = playerTile;
					{
						auto tmp = nextTile;
						switch (player.direction) {
						case Direction::up:
							tmp = { nextTile.x, nextTile.y - 1 };
							break;
						case Direction::down:
							tmp = { nextTile.x, nextTile.y + 1 };
							break;
						case Direction::left:
							tmp = { nextTile.x - 1, nextTile.y };
							break;
						case Direction::right:
							tmp = { nextTile.x + 1, nextTile.y };
							break;
						}
						if (valid(tmp))
							nextTile = tmp;
					}

					while (potentialDistance > 0) {
						bool moved = 0;
						int i = 0;
						if (nextTile.x * TILESIZE > position.x + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							++position.x;
							moved = true; ++i;
						}
						else if (nextTile.x * TILESIZE < position.x + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							--position.x; moved = true; ++i;
						}
						if (nextTile.y  * TILESIZE > position.y + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							++position.y; moved = true; ++i;
						}
						else if (nextTile.y * TILESIZE < position.y + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							--position.y; moved = true; ++i;
						}
						--potentialDistance;
						if (i >= 2)
							std::cout << "Saved a pixel due to cornering" << std::endl;

						if (!moved)
							break;

						if (position.x >= 28 * TILESIZE)
							position.x = -TILESIZE;
						else if (position.x <= -TILESIZE)
							position.x = 28 * TILESIZE;
					}
				}
			}
		);
	};

	std::vector<std::vector<int>> layout;


private:
	bool valid(Tile tile) {
		tile.x = tile.x < 0 ? layout[0].size() + tile.x : tile.x;
		tile.x = tile.x >= layout[0].size() ? tile.x % layout[0].size() : tile.x;

		return (layout[tile.y][tile.x] == 0);
	}
};