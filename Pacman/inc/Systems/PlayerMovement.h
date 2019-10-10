#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class PlayerMovement : public bloom::systems::System {
	using Position = bloom::components::Position;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	void update(double deltaTime = 0) override {
		m_registry.view<Pacman, Position>().each(
			[&](auto entity, Pacman& player, Position& position) {
				int potentialDistance = 0;
				player.timeAvailable += deltaTime / 1000.0;

				potentialDistance = static_cast<int>(player.timeAvailable * player.currSpeed);
				player.timeAvailable -= potentialDistance / player.currSpeed;

				Tile playerTile{ (position.x + PACMAN_TEXTURESIZE / 2) / TILESIZE, (position.y + PACMAN_TEXTURESIZE / 2) / TILESIZE };

				if (playerTile.x < 27 || playerTile.x >= 0) {
					// Change anim
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

					auto oldPos = position;
					bool moved = false;
					while (potentialDistance > 0) {
						moved = false;
						auto newPos = position;
						if (nextTile.x * TILESIZE > position.x + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							++newPos.x;
						}
						else if (nextTile.x * TILESIZE < position.x + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							--newPos.x;
						}
						if (nextTile.y * TILESIZE > position.y + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							++newPos.y;
						}
						else if (nextTile.y * TILESIZE < position.y + (PACMAN_TEXTURESIZE - TILESIZE) / 2) {
							--newPos.y;
						}


						if (newPos.x != position.x || newPos.y != position.y) {
							moved = true;
							position = newPos;
							--potentialDistance;
							m_registry.get<bloom::graphics::AnimationPtr>(entity)->update(player.currSpeed);
						}

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

	std::array<std::array<int, 31>, 28> layout;


private:
	bool valid(Tile tile) {
		tile.x = tile.x < 0 ? layout.size() + tile.x : tile.x;
		tile.x = tile.x >= layout.size() ? tile.x % layout.size() : tile.x;

		return layout[tile.x][tile.y] == 0;
	}
};