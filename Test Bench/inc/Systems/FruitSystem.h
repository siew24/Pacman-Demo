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

				playerState.score += static_cast<int>(fruit.type);

				m_registry.destroy(entity);
			}
			else
			{
				if (fruit.timeSpawned / 1000 > 9 && fruit.timeSpawned / 1000 < 10)
					m_registry.destroy(entity);
				else
					fruit.timeSpawned += deltaTime.value();
			}
		}
		);
	}
};