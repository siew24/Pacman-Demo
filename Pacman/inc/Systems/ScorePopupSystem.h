#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"


class ScorePopupSystem : public bloom::systems::System {
	using Sprite = bloom::components::Sprite;
	using bloom::systems::System::DefaultSystem;

public:
	void update(double deltaTime = 0) override {
		freeze = false;
		m_registry.view<ScoreComponent, Position, Size>().each(
			[&](auto entity, ScoreComponent & popup, Position & position, Size & size) {
				popup.currentTime += deltaTime / 1000.0;
				if (popup.currentTime >= popup.lifespan)
					if (m_registry.has<Ghost>(entity)) {
						m_registry.get<Position>(entity) = popup.oldPosition;
						m_registry.get<Size>(entity) = popup.oldSize;
						m_registry.remove<ScoreComponent>(entity);
					}
					else
						m_registry.destroy(entity);
				else {
					if (m_registry.has<Ghost>(entity)) freeze = true;
				}
			}
		);
	}

	bool freeze = false;
};