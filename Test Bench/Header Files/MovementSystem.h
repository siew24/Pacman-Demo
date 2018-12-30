#pragma once

#include "Framework.h"
#include "Level.h"
#include "Components/ComponentIncludes.h"

// focusing on Pacman for now
class MovementSystem : bloom::systems::DefaultSystem
{
	using AnimationPtr = bloom::components::AnimationPtr;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

	public:
		void update(std::optional<double> deltaTime = 0.0) override {
			m_registry.view<Direction>().each(
				[&](auto entity, Direction& direction) {
				if (m_registry.has<Pacman>(entity)) {
					Wall_down = false;
					Wall_left = false;
					Wall_right = false;
					Wall_up = false;
					auto & entity_pos = m_registry.get<Position>(entity);
					auto & animSet = m_registry.get<AnimationSet>(entity);
					auto & tmp = entity_pos;
					m_registry.view<Wall>().each(
						[&](auto wallID, Wall& wall) {
						if (Wall_left == false && wall.WallPosition.x == tmp.x - 1)
							Wall_left = true;
						if (Wall_right == false && wall.WallPosition.x == tmp.x + 1)
							Wall_right = true;
						if (Wall_up == false && wall.WallPosition.x == tmp.y - 1)
							Wall_up = true;
						if (Wall_down == false && wall.WallPosition.x == tmp.y + 1)
							Wall_down = true;
					}
					);
					if (direction.direction == "up" && !Wall_up)
						tmp.y -= 2;
					else if (direction.direction == "down" && !Wall_down)
						tmp.y += 2;
					else if (direction.direction == "left" && !Wall_left)
						tmp.x -= 2;
					else if (direction.direction == "right" && !Wall_right)
						tmp.x += 2;
					entity_pos = tmp;
					if (entity_pos.x <= 0)
						entity_pos.x = 27 * 19;
					else if (entity_pos.x > 27 * 19)
						entity_pos.x = 0;
					animSet.changeAnimation(direction.direction);
					}
			}
			);
		}

	private:
		bool Wall_left = false;
		bool Wall_right = false;
		bool Wall_up = false;
		bool Wall_down = false;

		std::vector<std::string> animations{
			"up",
			"down",
			"left",
			"right"
		};
};