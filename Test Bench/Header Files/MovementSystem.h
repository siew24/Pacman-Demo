#pragma once

#include "Framework.h"
#include "Components/DirectionComponent.h"

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
					auto & entity_pos = m_registry.get<Position>(entity);
					auto & animSet = m_registry.get<AnimationSet>(entity);
					auto & tmp = entity_pos;
					std::clog << direction.direction << "\n";
					if (direction.direction == "up")
						tmp.y -= 2;
					else if (direction.direction == "down")
						tmp.y += 2;
					else if (direction.direction == "left")
						tmp.x -= 2;
					else if (direction.direction == "right")
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
		std::vector<std::string> animations{
			"up",
			"down",
			"left",
			"right"
		};
};