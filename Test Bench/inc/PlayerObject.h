#pragma once
#include "GameObject.h"
#include "Configs.h"

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

	void init(const std::filesystem::path texturePath) {
		m_registry.replace<Position>(m_entity, 13* TILESIZE, 23* TILESIZE);
		m_registry.assign<Size>(m_entity, TILESIZE, TILESIZE);
		auto tmp = m_gameInstance->textures.load(texturePath);

		m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 13 * TILESIZE,23 * TILESIZE,TILESIZE,TILESIZE });

		AnimationPtr down = std::make_shared<Animation>();
		down->animationFrames = {
			Sprite(tmp, SDL_Rect{ 24,0,11,11 }),
			Sprite(tmp, SDL_Rect{ 36,0,11,11 })
		};


		AnimationPtr up = std::make_shared<Animation>();
		up->animationFrames = {
			Sprite(tmp, SDL_Rect{ 72,0,11,11 }),
			Sprite(tmp, SDL_Rect{ 84,0,11,11 })
		};

		AnimationPtr left = std::make_shared<Animation>();
		left->animationFrames = {
			Sprite(tmp, SDL_Rect{ 48,0,11,11 }),
			Sprite(tmp, SDL_Rect{ 60,0,11,11 })
		};

		AnimationPtr right = std::make_shared<Animation>();
		right->animationFrames = {
			Sprite(tmp, SDL_Rect{ 0,0,11,11 }),
			Sprite(tmp, SDL_Rect{ 12,0,11,11 })
		};

		up->setFrameTime(150);
		down->setFrameTime(150);
		left->setFrameTime(150);
		right->setFrameTime(150);

		AnimationSet animSet;
		animSet.addAnimation("up", up);
		animSet.addAnimation("down", down);
		animSet.addAnimation("left", left);
		animSet.addAnimation("right", right);

		m_registry.assign<AnimationSet>(m_entity, animSet);
		m_registry.assign<AnimationPtr>(m_entity, right);
		m_registry.assign<Pacman>(m_entity) = Pacman{Direction::null,Direction::null,0,0 };
	}
};