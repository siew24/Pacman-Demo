#pragma once

#include "Framework.h"
#include "Components/ComponentIncludes.h"

class TileObject : public bloom::GameObject
{
	// This class is used to initialize every wall parts as entities
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using bloom::GameObject::GameObject;

	public:
		void init() override {}

		void init(std::filesystem::path WallTexture, SDL_Rect destRect = { 0, 0, 256, 256 }, SDL_Rect srcRect = { 0, 0, 256, 256 })
		{
			m_registry.replace<Position>(m_entity, destRect.x, destRect.y);
			m_registry.assign<Size>(m_entity, destRect.w, destRect.h);
			auto tmp = m_gameInstance->textures.load(WallTexture);

			m_registry.assign<Sprite>(m_entity, tmp, srcRect);
			m_registry.assign<NoRandomPos>(m_entity);
		}

		void isWall()
		{
			m_registry.assign<Wall>(m_entity, m_registry.get<Position>(m_entity));
		}

		void isPellet()
		{
			m_registry.assign<Pellet>(m_entity, m_registry.get<Position>(m_entity));
		}
};