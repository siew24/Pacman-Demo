#pragma once
#include "Systems/Systems.h"
#include "../Components/ComponentIncludes.h"
#include "../Configs.h"

class GameDirectorSystem : public bloom::systems::System {
	using Position = bloom::components::Position;
	using bloom::systems::System::DefaultSystem;

public:
	void init() {
		m_registry.view<Pacman>().each([&](auto entity, auto & pac) { player = &pac; });
		m_registry.view<entt::label<"speedy"_hs>, Ghost>().each([&](auto entity, auto label, auto & ghost) { speedy = &ghost; });
		m_registry.view<entt::label<"bashful"_hs>, Ghost>().each([&](auto entity, auto label, auto & ghost) { bashful = &ghost; });
		m_registry.view<entt::label<"pokey"_hs>, Ghost>().each([&](auto entity, auto label, auto & ghost) { pokey = &ghost; });
	}
	void setParameters(bloom::Game*& Game) {
		m_gameInstance = Game;
	}
	void setParameters(const std::filesystem::path& assetPath, FruitType fruit) {
		texturePath = assetPath;
		m_fruitType = fruit;
	}
	void update(std::optional<double> deltaTime = std::nullopt) override {
		if (player->died) {
			bool timeout = false;
			if (player->timeFromLastPellet >= ((levelNumber == 0) ? 4.0 : 3.0))
				player->timeFromLastPellet = std::fmod(player->timeFromLastPellet, ((levelNumber == 0) ? 4.0 : 3.0)), timeout = true;
			if ((player->pelletsEaten >= 7 || timeout) && !speedy->released)
				speedy->released = true;
			else if ((player->pelletsEaten >= 17 || timeout) && !bashful->released)
				bashful->released = true;
			else if ((player->pelletsEaten >= 32 || timeout) && !pokey->released)
				pokey->released = true;
		}
		else {
			speedy->released = true;
			if (player->pelletsEaten >= bashful->levelVars.dotLimit)
				bashful->released = true;
			if (player->pelletsEaten >= pokey->levelVars.dotLimit)
				pokey->released = true;
		}
		if (player->pelletsEaten == 70 || player->pelletsEaten == 170) {
			if (fruitEntityPtr) {
				fruitEntityPtr.reset();
			}
			auto fruit = std::make_shared<FruitObject>(m_registry, m_gameInstance);
			fruit->init(texturePath / "Entity", m_fruitType);
			fruitEntityPtr = fruit;
		}
	}

	int levelNumber = 0;

private:
	Pacman* player;
	Ghost* speedy;
	Ghost* bashful;
	Ghost* pokey;

	bloom::Game* m_gameInstance;
	std::filesystem::path texturePath;

	std::shared_ptr<bloom::GameObject> fruitEntityPtr;

	FruitType m_fruitType;
};