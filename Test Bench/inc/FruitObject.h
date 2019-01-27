#pragma once

#include "GameObject.h"
#include "Components/ComponentIncludes.h"
#include "Configs.h"

class FruitObject : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using bloom::GameObject::GameObject;
public:
	void init() override {}

	void init(std::filesystem::path texture, SDL_Rect destRect = { 0, 0, TILESIZE,TILESIZE })
	{
		m_registry.replace<Position>(m_entity, destRect.x, destRect.y);
		m_registry.assign<Size>(m_entity, destRect.w, destRect.h);
		auto tmp = m_gameInstance->textures.load(texture);

		if (texture.filename().u8string() == "41.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.accommodate<entt::label<"cherry"_hs>>(m_entity);
		}
		else if(texture.filename().u8string() == "42.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.accommodate<entt::label<"peach"_hs>>(m_entity);
		}
		else if(texture.filename().u8string() == "43.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.accommodate<entt::label<"apple"_hs>>(m_entity);
		}
		else if (texture.filename().u8string() == "44.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,12 });
			m_registry.accommodate<entt::label<"strawberry"_hs>>(m_entity);
		}
		else if (texture.filename().u8string() == "45.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,11 });
			m_registry.accommodate<entt::label<"galaxian"_hs>>(m_entity);
		}
		else if (texture.filename().u8string() == "46.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,13 });
			m_registry.accommodate<entt::label<"bell"_hs>>(m_entity);
		}
		else if (texture.filename().u8string() == "47.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,13 });
			m_registry.accommodate<entt::label<"grape"_hs>>(m_entity);
		}
		else if (texture.filename().u8string() == "48.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,7,13 });
			m_registry.accommodate<entt::label<"strawberry"_hs>>(m_entity);
		}

		m_registry.assign<Fruit>(m_entity);
	}
};