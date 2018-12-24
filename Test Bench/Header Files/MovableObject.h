#pragma once

#include "Framework.h"
#include "NoRandomComponent.h"
#include <iostream>

class MovableObject : public bloom::GameObject
{
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using bloom::GameObject::GameObject;

	public:
		void init() override {}

		void init(SDL_Rect pos_and_size, const std::filesystem::path texturePath = "Assets/Pacman_Spritesheet.png", std::optional<SDL_Rect> srcRect = std::nullopt) 
		{
			m_registry.replace<Position>(m_entity, pos_and_size.x, pos_and_size.y);
			m_registry.assign<Size>(m_entity, pos_and_size.w, pos_and_size.h);
			auto tmp = m_gameInstance->textures.load(texturePath);

			m_registry.assign<Sprite>(m_entity, tmp, srcRect);
		}

		void disableRandomPos() {
			m_registry.assign<NoRandomPos>(m_entity);
		}

		void enableRandomPos() {
			m_registry.reset<NoRandomPos>(m_entity);
		}
};