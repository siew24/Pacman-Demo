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

		if (texture.filename().u8string() == "Cherry.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.assign<Fruit>(m_entity).type = cherry;
		}
		else if(texture.filename().u8string() == "Peach.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.assign<Fruit>(m_entity).type = peach;
		}
		else if(texture.filename().u8string() == "Apple.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,12 });
			m_registry.assign<Fruit>(m_entity).type = apple;
		}
		else if (texture.filename().u8string() == "Strawberry.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,12 });
			m_registry.assign<Fruit>(m_entity).type = strawberry;
		}
		else if (texture.filename().u8string() == "Galaxian.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,11 });
			m_registry.assign<Fruit>(m_entity).type = galaxian;
		}
		else if (texture.filename().u8string() == "Bell.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,12,13 });
			m_registry.assign<Fruit>(m_entity).type = bell;
		}
		else if (texture.filename().u8string() == "Grape.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,11,13 });
			m_registry.assign<Fruit>(m_entity).type = grape;
		}
		else if (texture.filename().u8string() == "Key.png")
		{
			m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,7,13 });
			m_registry.assign<Fruit>(m_entity).type = key;
		}

		
	}
};