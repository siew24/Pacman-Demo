#pragma once
#include "GameObject.h"
#include "Configs.h"
#include "Components/GhostComponent.h"
#include "GhostBehaviors/GhostBehavior.h"

class GhostObject : public bloom::GameObject {
	using Position = bloom::components::Position;
	using Size = bloom::components::Size;
	using Sprite = bloom::components::Sprite;
	using AnimationPtr = bloom::components::AnimationPtr;
	using AnimationSet = bloom::components::AnimationSet;
	using Animation = bloom::components::Animation;
	using bloom::GameObject::GameObject;
public:
	void init() override {}

	void init(const std::filesystem::path texturePath, Ghosts id, Tile spawnTile) {
		m_registry.replace<Position>(m_entity, spawnTile.x * TILESIZE, spawnTile.y * TILESIZE);
		m_registry.accommodate<Size>(m_entity, TILESIZE, TILESIZE);
		bloom::graphics::TexturePtr tmp;

		if (id == shadow) {
			m_registry.accommodate<entt::label<"shadow"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ ghostBehaviors::shadow, spawnTile };
			tmp = m_gameInstance->textures.load(texturePath / "Red.png");
		}
		else if (id == speedy) {
			m_registry.accommodate<entt::label<"speedy"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ ghostBehaviors::speedy, spawnTile };
			tmp = m_gameInstance->textures.load(texturePath / "Pinky.png");
		}
		else if (id == bashful) {
			m_registry.accommodate<entt::label<"bashful"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ ghostBehaviors::bashful, spawnTile };
			tmp = m_gameInstance->textures.load(texturePath / "Blue.png");
		}else if (id == pokey) {
			m_registry.accommodate<entt::label<"pokey"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ ghostBehaviors::pokey, spawnTile };
			tmp = m_gameInstance->textures.load(texturePath / "Orange.png");
		}

		auto tmp2 = m_gameInstance->textures.load(texturePath / "Ghosts_afraid.png");
		auto tmp3 = m_gameInstance->textures.load(texturePath / "Ghosts_eaten.png");

		m_registry.accommodate<Sprite>(m_entity, tmp, SDL_Rect{ spawnTile.x * TILESIZE, spawnTile.y * TILESIZE,TILESIZE,TILESIZE });

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
		AnimationPtr afraid = std::make_shared<Animation>();
		afraid->animationFrames = {
			Sprite(tmp2, SDL_Rect{ 72,48,11,11 }),
			//Sprite(tmp2, SDL_Rect{ 96,0,11,11 })
		};

		AnimationPtr downd = std::make_shared<Animation>();
		downd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 24,0,11,11 }),
			Sprite(tmp3, SDL_Rect{ 36,0,11,11 })
		};


		AnimationPtr upd = std::make_shared<Animation>();
		upd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 72,0,11,11 }),
			Sprite(tmp3, SDL_Rect{ 84,0,11,11 })
		};

		AnimationPtr leftd = std::make_shared<Animation>();
		leftd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 48,0,11,11 }),
			Sprite(tmp3, SDL_Rect{ 60,0,11,11 })
		};

		AnimationPtr rightd = std::make_shared<Animation>();
		rightd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 0,0,11,11 }),
			Sprite(tmp3, SDL_Rect{ 12,0,11,11 })
		};

		up->setFrameTime(150);
		down->setFrameTime(150);
		left->setFrameTime(150);
		right->setFrameTime(150);
		upd->setFrameTime(150);
		downd->setFrameTime(150);
		leftd->setFrameTime(150);
		rightd->setFrameTime(150);
		afraid->setFrameTime(500);

		AnimationSet animSet;
		animSet.addAnimation("up", up);
		animSet.addAnimation("down", down);
		animSet.addAnimation("left", left);
		animSet.addAnimation("right", right);
		animSet.addAnimation("upd", upd);
		animSet.addAnimation("downd", downd);
		animSet.addAnimation("leftd", leftd);
		animSet.addAnimation("rightd", rightd);
		animSet.addAnimation("afraid", afraid);

		m_registry.accommodate<AnimationSet>(m_entity, animSet);
		m_registry.accommodate<AnimationPtr>(m_entity, right);
	}
};