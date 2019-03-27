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

	void init(std::filesystem::path texture, FruitType fruit)
	{
		SDL_Rect destRect{ FRUIT_POS_X * TILESIZE - 4, FRUIT_POS_Y*TILESIZE};
		
		switch (fruit) {
		default:
		case FruitType::cherry:
			texture /= "Cherry.png";
			destRect.x -= (12 - TILESIZE) / 2, destRect.y -= (12 - TILESIZE) / 2;
			destRect.w = 12; destRect.h = 12;
			break;
		case FruitType::peach:
			texture /= "Peach.png";
			destRect.x -= (12 - TILESIZE) / 2, destRect.y -= (12 - TILESIZE) / 2;
			destRect.w = 12, destRect.h = 12;
			break;
		case FruitType::apple:
			texture /= "Apple.png";
			destRect.x -= (12 - TILESIZE) / 2, destRect.y -= (12 - TILESIZE) / 2;
			destRect.w = 12, destRect.h = 12;
			break;
		case FruitType::strawberry:
			texture /= "Strawberry.png";
			destRect.x -= (11 - TILESIZE) / 2, destRect.y -= (12 - TILESIZE) / 2;
			destRect.w = 11, destRect.h = 12;
			break;
		case FruitType::galaxian:
			texture /= "Galaxian.png";
			destRect.x -= (11 - TILESIZE) / 2, destRect.y -= (11 - TILESIZE) / 2;
			destRect.w = 11, destRect.h = 11;
			break;
		case FruitType::bell:
			texture /= "Bell.png";
			destRect.x -= (12 - TILESIZE) / 2, destRect.y -= (13 - TILESIZE) / 2;
			destRect.w = 12, destRect.h = 13;
			break;
		case FruitType::grape:
			texture /= "Grape.png";
			destRect.x -= (11 - TILESIZE) / 2, destRect.y -= (13 - TILESIZE) / 2;
			destRect.w = 11, destRect.h = 13;
			break;
		case FruitType::key:
			texture /= "Key.png";
			destRect.x -= (7 - TILESIZE) / 2, destRect.y -= (13 - TILESIZE) / 2;
			destRect.w = 7, destRect.h = 13;
			break;
		}

		m_registry.replace<Position>(m_entity, destRect.x, destRect.y);
		auto tmp = m_gameInstance->textures.load(texture);
		m_registry.assign<Sprite>(m_entity, tmp, SDL_Rect{ 0,0,destRect.w,destRect.h });
		m_registry.assign<Size>(m_entity, destRect.w, destRect.h);
		
		Fruit& fruitComp = m_registry.assign<Fruit>(m_entity);
		fruitComp.type = fruit;
		fruitComp.rect = destRect;
	}
};