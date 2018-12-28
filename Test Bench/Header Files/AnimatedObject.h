#pragma once

#include "Framework.h"
#include "Components/PacmanComponent.h"
#include "Components/GhostComponent.h"
#include "Components/NoRandomComponent.h"

class AnimatedObject : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using AnimationPtr = bloom::components::AnimationPtr;
	using Animation = bloom::components::Animation;
	using AnimationSet = bloom::components::AnimationSet;
	using bloom::GameObject::GameObject;

public:
	void init() override {}

	void init(const std::filesystem::path texturePath, SDL_Rect pos_and_size = {0, 0, 256, 256}) {
		m_registry.replace<Position>(m_entity, pos_and_size.x, pos_and_size.y);
		m_registry.assign<Size>(m_entity, pos_and_size.w, pos_and_size.h);
		auto tmp = m_gameInstance->textures.load(texturePath);

		m_registry.assign<Sprite>(m_entity, tmp, pos_and_size);

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
		m_registry.assign<AnimationPtr>(m_entity, up);
		m_registry.assign<NoRandomPos>(m_entity);
	}

	void isPacman()
	{
		m_registry.assign<Pacman>(m_entity);
	}

	void isGhost()
	{
		m_registry.assign<Ghosts>(m_entity);
	}
};