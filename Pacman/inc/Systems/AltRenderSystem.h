#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class AltRenderSystem : public bloom::systems::System {
	using Sprite = bloom::components::Sprite;
	using bloom::systems::System::DefaultSystem;

public:
	void update(double = 0) override {
		m_registry.group<>(entt::get<Position, Size, Sprite>, entt::exclude<Eaten>).each(
			[&](auto entity, Position& pos, Size& size, Sprite& spr) {
				SDL_Rect destRect{
					static_cast<int>(pos.x),
					static_cast<int>(pos.y),
					static_cast<int>(size.w),
					static_cast<int>(size.h)
				};
				spr.texture->render(spr.srcRect, destRect);
			});
	}

	bool enabled = true;
};