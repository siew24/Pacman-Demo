#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class AltAnimationSystem : public bloom::systems::System {
	using AnimationPtr = bloom::components::AnimationPtr;
	using AnimationSet = bloom::components::AnimationSet;
	using Sprite = bloom::components::Sprite;
	using bloom::systems::System::DefaultSystem;

public:
	void update(std::optional<double> deltaTime = 0.0) override {
		m_registry.view<AnimationPtr>().each(
			[&](auto entity, AnimationPtr & anim) {
				if (m_registry.has<AnimationSet>(entity)) {
					AnimationPtr newAnim = m_registry.get<AnimationSet>(entity).getCurrent();
					if (newAnim)
						anim = newAnim;
				}
				if (m_registry.has<Pacman>(entity))
					m_registry.replace<Sprite>(entity, anim->update(0.0));
				else
					m_registry.replace<Sprite>(entity, anim->update(deltaTime.value()));
			}
		);
	}
};