#pragma once

#include "Framework.h"
#include "NoRandomComponent.h"

class TestAnimChar : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using AnimationPtr = bloom::components::AnimationPtr;
	using Animation = bloom::components::Animation;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::GameObject::GameObject;

public:
	void init() override {}

	void init(const std::filesystem::path texturePath = "Assets/Pacman_Sprite.png") {
		m_registry.replace<Position>(m_entity, 50, 50);
		m_registry.assign<Size>(m_entity, 256, 256);
		auto tmp = m_gameInstance->textures.load(texturePath);

		m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,32,32,32 });


		// Seriously not the best way to initialize object animation.
		AnimationPtr down = std::make_shared<Animation>();
		down->animationFrames = {
			Sprite(tmp, SDL_Rect{ 60,35,12,12 }),
			Sprite(tmp, SDL_Rect{ 84,35,12,12 })
		};


		AnimationPtr up = std::make_shared<Animation>();
		up->animationFrames = {
			Sprite(tmp, SDL_Rect{ 12,35,12,12 }),
			Sprite(tmp, SDL_Rect{ 36,35,12,12 })
		};

		AnimationPtr left = std::make_shared<Animation>();
		left->animationFrames = {
			Sprite(tmp, SDL_Rect{ 0,36,12,12 }),
			Sprite(tmp, SDL_Rect{ 24,36,12,12 })
		};

		AnimationPtr right = std::make_shared<Animation>();
		right->animationFrames = {
			Sprite(tmp, SDL_Rect{ 49,35,12,12 }),
			Sprite(tmp, SDL_Rect{ 73,35,12,12 })
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
		m_registry.assign<AnimationPtr>(m_entity, up);
		m_registry.assign<NoRandomPos>(m_entity);
	}
};
