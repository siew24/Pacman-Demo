#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class FruitSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		entt::DefaultRegistry::entity_type player;
		m_registry.view<Pacman>().each([&](auto entity, Pacman& pac) { player = entity; });

		//We do not want to keep fetching this, so lets cache this info here.
		Position& playerPos = m_registry.get<Position>(player);
		Pacman& playerState = m_registry.get<Pacman>(player);


		m_registry.view<Fruit, Position>().each(
			[&](auto entity, Fruit& fruit, Position& fruitPos) {
			if (((playerPos.x + (ENTITYSIZE / 2)) / TILESIZE == fruitPos.x / TILESIZE) && ((playerPos.y + (ENTITYSIZE / 2)) / TILESIZE == fruitPos.y / TILESIZE)) {

				if(m_registry.has<entt::label<"cherry"_hs>>(entity))
					playerState.score += 100;
				if (m_registry.has<entt::label<"strawberry"_hs>>(entity))
					playerState.score += 300;
				if (m_registry.has<entt::label<"peach"_hs>>(entity))
					playerState.score += 500;
				if (m_registry.has<entt::label<"apple"_hs>>(entity))
					playerState.score += 700;
				if (m_registry.has<entt::label<"grape"_hs>>(entity))
					playerState.score += 1000;
				if (m_registry.has<entt::label<"galaxian"_hs>>(entity))
					playerState.score += 2000;
				if (m_registry.has<entt::label<"bell"_hs>>(entity))
					playerState.score += 3000;
				if (m_registry.has<entt::label<"key"_hs>>(entity))
					playerState.score += 5000;

				m_registry.destroy(entity);
			}
		}
		);
	}
};