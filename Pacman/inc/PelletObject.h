#pragma once

#include "GameObject.h"
#include "Components/ComponentIncludes.h"
#include "Components/Animation.h"
#include "Configs.h"

class PelletObject : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using AnimationPtr = bloom::components::AnimationPtr;
	using bloom::GameObject::GameObject;

public:
	void init() override {}

	void init(std::filesystem::path texture, SDL_Rect destRect = { 0, 0, TILESIZE,TILESIZE })
	{
		m_registry.replace<Position>(m_entity, destRect.x, destRect.y);
		m_registry.assign<Size>(m_entity, destRect.w, destRect.h);
		auto tmp = m_gameInstance->textures.load(texture);

		m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,TILETEXTURESIZE,TILETEXTURESIZE });
		if (texture.filename().u8string() == "Pellet.png") {
			m_registry.assign<Pellet>(m_entity);
		}
		else if (texture.filename().u8string() == "PowerPellet.png") {
			m_registry.assign<PowerPellet>(m_entity);
			AnimationPtr blink = std::make_shared<Animation>();
			blink->animationFrames = {
				Sprite(tmp, SDL_Rect{ 0, 0, 8, 8}),
				Sprite(tmp, SDL_Rect{ 0, 0, 1, 1})
			};
			blink->setFrameTime(200);

			m_registry.assign<AnimationPtr>(m_entity, blink);
		}
	}
};