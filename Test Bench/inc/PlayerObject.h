#pragma once
#include "GameObject.h"
#include "Configs.h"
#include "Tile.h"

class Player : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using AnimationPtr = bloom::components::AnimationPtr;
	using AnimationSet = bloom::components::AnimationSet;
	using Animation = bloom::components::Animation;
	using bloom::GameObject::GameObject;
public:
	void init() override {}

	void init(const std::filesystem::path texturePath, Tile spawnTile) {
		m_registry.replace<Position>(m_entity, spawnTile.x * TILESIZE, spawnTile.y * TILESIZE);
		m_registry.accommodate<Size>(m_entity, TILESIZE, TILESIZE);
		m_registry.accommodate<entt::label<"pacman"_hs>>(m_entity);
		auto tmp = m_gameInstance->textures.load(texturePath);

		m_registry.accommodate<Sprite>(m_entity, tmp, SDL_Rect{ spawnTile.x * TILESIZE, spawnTile.y * TILESIZE,TILESIZE,TILESIZE });

		AnimationPtr down = std::make_shared<Animation>();
		down->animationFrames = {
			Sprite(tmp, SDL_Rect{ 2 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE }),
			Sprite(tmp, SDL_Rect{ 3 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE })
		};


		AnimationPtr up = std::make_shared<Animation>();
		up->animationFrames = {
			Sprite(tmp, SDL_Rect{ 6 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE }),
			Sprite(tmp, SDL_Rect{ 7 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE })
		};

		AnimationPtr left = std::make_shared<Animation>();
		left->animationFrames = {
			Sprite(tmp, SDL_Rect{ 4 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE }),
			Sprite(tmp, SDL_Rect{ 5 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE })
		};

		AnimationPtr right = std::make_shared<Animation>();
		right->animationFrames = {
			Sprite(tmp, SDL_Rect{ 0 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE }),
			Sprite(tmp, SDL_Rect{ 1 * ENTITYSIZE,0,ENTITYSIZE,ENTITYSIZE })
		};

		up->setFrameTime(ENTITYFRAMETIME);
		down->setFrameTime(ENTITYFRAMETIME);
		left->setFrameTime(ENTITYFRAMETIME);
		right->setFrameTime(ENTITYFRAMETIME);

		AnimationSet animSet;
		animSet.addAnimation("up", up);
		animSet.addAnimation("down", down);
		animSet.addAnimation("left", left);
		animSet.addAnimation("right", right);

		m_registry.accommodate<AnimationSet>(m_entity, animSet);
		m_registry.accommodate<AnimationPtr>(m_entity, right);
		if (!m_registry.has<Pacman>(m_entity))
			m_registry.assign<Pacman>(m_entity) = Pacman{ Direction::null,Direction::null,0,0 };
		else {
			auto& pac = m_registry.get<Pacman>(m_entity);
			pac = Pacman{ null,null,0,0,pac.score };
		}
	}
};