#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"

class AnimationChangerSystem : public bloom::systems::System {
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::systems::System::DefaultSystem;

public:
	void update(std::optional<double> deltaTime = 0.0) override {
		m_registry.view<AnimationSet, Ghost>().each(
			[&](auto entity, AnimationSet & animSet, Ghost & ghost) {
				std::string animset = "";
				switch (ghost.direction) {
				case Direction::left:
					animset += "left";
					break;
				case Direction::right:
					animset += "right";
					break;
				case Direction::up:
					animset += "up";
					break;
				case Direction::down:
					animset += "down";
					break;
				default:
					animset += "left";
					break;
				}
				switch (ghost.currentMode) {
				case BehaviourModes::dead:
					animset += "d";
					break;
				case BehaviourModes::afraid:
					animset = (ghost.afraidTimer <= ghost.levelVars.flashAmount * (ENTITYFRAMETIME / 1000.0) * 4) ? "afraidFlash" : "afraid";
					break;
				}
				animSet.changeCurrent(animset);
			}
		);
		m_registry.view<AnimationSet, Pacman>().each(
			[&](auto entity, AnimationSet & animSet, Pacman & pacman) {
				switch (pacman.direction) {
				case Direction::right:
					animSet.changeCurrent("right");
					break;
				case Direction::left:
					animSet.changeCurrent("left");
					break;
				case Direction::up:
					animSet.changeCurrent("up");
					break;
				case Direction::down:
					animSet.changeCurrent("down");
					break;
				}
			}
		);
	}

	bool enabled = true;
};