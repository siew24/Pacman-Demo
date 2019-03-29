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

	void init(const std::filesystem::path texturePath, Ghosts id, Tile spawnTile, GhostInitDetails details) {
		m_registry.replace<Position>(m_entity, spawnTile.x * TILESIZE - 4 - (GHOST_TEXTURESIZE-TILESIZE)/2, spawnTile.y * TILESIZE - (GHOST_TEXTURESIZE - TILESIZE) / 2);
		m_registry.accommodate<Size>(m_entity, GHOST_TEXTURESIZE, GHOST_TEXTURESIZE);
		bloom::graphics::TexturePtr tmp;

		if (id == Ghosts::shadow) {
			m_registry.accommodate<entt::label<"shadow"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity)= Ghost{ 
				ghostBehaviors::shadow,
				Tile{spawnTile.x, spawnTile.y + 3},
				details,
				0,
				true,
				false 
			};
			tmp = m_gameInstance->textures.load(texturePath / "Red.png");
		}
		else if (id == Ghosts::speedy) {
			m_registry.accommodate<entt::label<"speedy"_hs>>(m_entity);
			auto ghost = m_registry.accommodate<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::speedy, 
				spawnTile,
				details,
				0, 
				true 
			};
			tmp = m_gameInstance->textures.load(texturePath / "Pinky.png");
		}
		else if (id == Ghosts::bashful) {
			m_registry.accommodate<entt::label<"bashful"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::bashful, 
				spawnTile ,
				details,
				30 
			};
			tmp = m_gameInstance->textures.load(texturePath / "Blue.png");
		}
		else if (id == Ghosts::pokey) {
			m_registry.accommodate<entt::label<"pokey"_hs>>(m_entity);
			m_registry.accommodate<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::pokey, 
				spawnTile,
				details,
				60 
			};
			tmp = m_gameInstance->textures.load(texturePath / "Orange.png");
		}

		auto tmp2 = m_gameInstance->textures.load(texturePath / "Ghosts_afraid.png");
		auto tmp3 = m_gameInstance->textures.load(texturePath / "Ghosts_eaten.png");

		m_registry.accommodate<Sprite>(m_entity, tmp, SDL_Rect{ spawnTile.x * TILESIZE, spawnTile.y * TILESIZE,TILESIZE,TILESIZE });

		AnimationPtr down = std::make_shared<Animation>();
		down->animationFrames = {
			Sprite(tmp, SDL_Rect{ 2 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 3 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr up = std::make_shared<Animation>();
		up->animationFrames = {
			Sprite(tmp, SDL_Rect{ 6 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 7 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr left = std::make_shared<Animation>();
		left->animationFrames = {
			Sprite(tmp, SDL_Rect{ 4 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 5 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr right = std::make_shared<Animation>();
		right->animationFrames = {
			Sprite(tmp, SDL_Rect{ 0 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp, SDL_Rect{ 1 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};
		
		AnimationPtr downd = std::make_shared<Animation>();
		downd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 2 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp3, SDL_Rect{ 3 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr upd = std::make_shared<Animation>();
		upd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 6 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp3, SDL_Rect{ 7 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr leftd = std::make_shared<Animation>();
		leftd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 4 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp3, SDL_Rect{ 5 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr rightd = std::make_shared<Animation>();
		rightd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 0 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp3, SDL_Rect{ 1 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};
		AnimationPtr afraid = std::make_shared<Animation>();
		afraid->animationFrames = {
			Sprite(tmp2, SDL_Rect{ 3 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp2, SDL_Rect{ 2 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};
		AnimationPtr afraidFlash = std::make_shared<Animation>();
		afraidFlash->animationFrames = {
			Sprite(tmp2, SDL_Rect{ 0 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp2, SDL_Rect{ 1 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp2, SDL_Rect{ 2 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE }),
			Sprite(tmp2, SDL_Rect{ 3 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};


		up->setFrameTime(ENTITYFRAMETIME);
		down->setFrameTime(ENTITYFRAMETIME);
		left->setFrameTime(ENTITYFRAMETIME);
		right->setFrameTime(ENTITYFRAMETIME);
		upd->setFrameTime(ENTITYFRAMETIME);
		downd->setFrameTime(ENTITYFRAMETIME);
		leftd->setFrameTime(ENTITYFRAMETIME);
		rightd->setFrameTime(ENTITYFRAMETIME);
		afraid->setFrameTime(ENTITYFRAMETIME);
		afraidFlash->setFrameTime(ENTITYFRAMETIME);

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
		animSet.addAnimation("afraidFlash", afraidFlash);

		m_registry.accommodate<AnimationSet>(m_entity, animSet);
		m_registry.accommodate<AnimationPtr>(m_entity, right);
	}
};