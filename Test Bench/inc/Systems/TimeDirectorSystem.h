#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class TimeDirectorSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		m_registry.view<Ghost>().each([&](auto entity, auto& ghost) {
			
			if (levelNumber == 1) {
				ghost.scatterTimer = (ghost.modeLooped > 2) ? 5.0 : 7.0;
				ghost.chaseTimer = (ghost.modeLooped > 3) ? INFINITY : 20.0;
			}
			else if (levelNumber >= 2 && levelNumber <= 4) {
				ghost.scatterTimer = (ghost.modeLooped > 3) ? 1 / 60 : ((ghost.modeLooped > 2) ? 5.0 : 7.0);
				ghost.chaseTimer = (ghost.modeLooped > 3) ? INFINITY : ((ghost.modeLooped > 2) ? 1033.0 : 20.0);
			}
			else {
				ghost.scatterTimer = (ghost.modeLooped > 3) ? 1 / 60 : 5.0;
				ghost.chaseTimer = (ghost.modeLooped > 3) ? INFINITY : ((ghost.modeLooped > 2) ? 1037.0 : 20.0);
			}
		}
		);
	}

	int levelNumber = 0;
};