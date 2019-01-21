#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class GameDirectorSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	void init() {
		m_registry.view<Pacman>().each([&](auto entity, auto& pac) { player = &pac; });
		m_registry.view<entt::label<"bashful"_hs>, Ghost>().each([&](auto entity,auto label, auto& ghost) { bashful = &ghost; });
		m_registry.view<entt::label<"pokey"_hs>, Ghost>().each([&](auto entity, auto label, auto& ghost) { pokey = &ghost; });
	}
	virtual void update(std::optional<double> deltaTime = std::nullopt) override {
		if (player->pelletsEaten == bashful->dotLimit)
			bashful->released = true;
		else if (player->pelletsEaten == pokey->dotLimit)
			pokey->released = true;
	}

private:
	Pacman* player;
	Ghost* bashful;
	Ghost* pokey;
};