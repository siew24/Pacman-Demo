#pragma once

#include "Framework.h"
#include "Components/ComponentIncludes.h"

class PelletObject : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using bloom::GameObject::GameObject;
public:
	void init() override {}

	void init(std::filesystem::path texture, SDL_Rect destRect = { 0, 0, 19,19})
	{
		m_registry.replace<Position>(m_entity, destRect.x, destRect.y);
		m_registry.assign<Size>(m_entity, destRect.w, destRect.h);
		auto tmp = m_gameInstance->textures.load(texture);

		m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,6,6 });
		if (texture.filename().u8string() == "37.png") {
			m_registry.assign<Pellet>(m_entity);
		}
		else if (texture.filename().u8string() == "38.png") {
			m_registry.assign<MegaPellet>(m_entity);
		}
	}
};