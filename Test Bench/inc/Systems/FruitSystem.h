#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class FruitSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

	public:
		void init() {
			m_registry.view<Pacman>().each([&](auto entity, auto& pac) { player = &pac; });
		}

		virtual void update(std::optional<double> deltaTime = std::nullopt) override {
			if (player->pelletsEaten == 70)
				;
		}

	private:
		Pacman* player;
};