#pragma once
#include "GameObject.h"
#include "Configs.h"
#include "Tile.h"

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

	void init(const std::filesystem::path texturePath, Tile spawnTile, std::array<double, 2> speedMultipliers) {
		m_registry.replace<Position>(m_entity, spawnTile.x * TILESIZE-2 -(PACMAN_TEXTURESIZE-TILESIZE)/2 , spawnTile.y * TILESIZE - (PACMAN_TEXTURESIZE - TILESIZE) / 2);
		m_registry.accommodate<Size>(m_entity, PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE);
		m_registry.accommodate<entt::label<"pacman"_hs>>(m_entity);
		auto tmp = m_gameInstance->textures.load(texturePath);

		m_registry.accommodate<Sprite>(m_entity, tmp, SDL_Rect{ 0  , 0  ,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE });

		AnimationPtr down = std::make_shared<Animation>();
		down->animationFrames = {
			Sprite(tmp, SDL_Rect{ 2 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 3 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE })
		};


		AnimationPtr up = std::make_shared<Animation>();
		up->animationFrames = {
			Sprite(tmp, SDL_Rect{ 6 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 7 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE })
		};

		AnimationPtr left = std::make_shared<Animation>();
		left->animationFrames = {
			Sprite(tmp, SDL_Rect{ 4 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 5 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE })
		};

		AnimationPtr right = std::make_shared<Animation>();
		right->animationFrames = {
			Sprite(tmp, SDL_Rect{ 0 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 1 * PACMAN_TEXTURESIZE,0,PACMAN_TEXTURESIZE,PACMAN_TEXTURESIZE })
		};

		up->setFrameTime(2);
		down->setFrameTime(2);
		left->setFrameTime(2);
		right->setFrameTime(2);

		AnimationSet animSet;
		animSet.add("up", up);
		animSet.add("down", down);
		animSet.add("left", left);
		animSet.add("right", right);

		m_registry.accommodate<AnimationSet>(m_entity, animSet);
		m_registry.accommodate<AnimationPtr>(m_entity, right);
		if (!m_registry.has<Pacman>(m_entity))
			m_registry.assign<Pacman>(m_entity) = Pacman{ Direction::null,Direction::null,0,0, speedMultipliers};
		else {
			auto& pac = m_registry.get<Pacman>(m_entity);
			pac = Pacman{ Direction::null,Direction::null,pac.score,0, speedMultipliers};
		}
	}
};