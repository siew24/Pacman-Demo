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
		m_registry.replace<Position>(m_entity, (spawnTile.x * TILESIZE) - (GHOST_TEXTURESIZE-TILESIZE)/2, spawnTile.y * TILESIZE - (GHOST_TEXTURESIZE - TILESIZE) / 2);
		m_registry.assign_or_replace<Size>(m_entity, GHOST_TEXTURESIZE, GHOST_TEXTURESIZE);
		bloom::graphics::TexturePtr tmp;
		ghostID = id;

		if (id == Ghosts::shadow) {
			m_registry.assign_or_replace<entt::tag<"shadow"_hs>>(m_entity);
			m_registry.assign_or_replace<Ghost>(m_entity)= Ghost{ 
				ghostBehaviors::shadow,
				Tile{spawnTile.x, spawnTile.y},
				details,
				true,
				false 
			};
			tmp = c_gameInstance->textures.load(texturePath / "Red.png");
		}
		else if (id == Ghosts::speedy) {
			m_registry.assign_or_replace<entt::tag<"speedy"_hs>>(m_entity);
			auto ghost = m_registry.assign_or_replace<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::speedy, 
				spawnTile,
				details,
				true 
			};
			tmp = c_gameInstance->textures.load(texturePath / "Pinky.png");
		}
		else if (id == Ghosts::bashful) {
			m_registry.assign_or_replace<entt::tag<"bashful"_hs>>(m_entity);
			m_registry.assign_or_replace<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::bashful, 
				spawnTile ,
				details
			};
			tmp = c_gameInstance->textures.load(texturePath / "Blue.png");
		}
		else if (id == Ghosts::pokey) {
			m_registry.assign_or_replace<entt::tag<"pokey"_hs>>(m_entity);
			m_registry.assign_or_replace<Ghost>(m_entity) = Ghost{ 
				ghostBehaviors::pokey, 
				spawnTile,
				details
			};
			tmp = c_gameInstance->textures.load(texturePath / "Orange.png");
		}

		auto tmp2 = c_gameInstance->textures.load(texturePath / "Ghosts_afraid.png");
		auto tmp3 = c_gameInstance->textures.load(texturePath / "Ghosts_eaten.png");

		m_registry.assign_or_replace<Sprite>(m_entity, tmp, SDL_Rect{ spawnTile.x * TILESIZE, spawnTile.y * TILESIZE,TILESIZE,TILESIZE });

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
			Sprite(tmp3, SDL_Rect{ 6 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr leftd = std::make_shared<Animation>();
		leftd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 4 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
		};

		AnimationPtr rightd = std::make_shared<Animation>();
		rightd->animationFrames = {
			Sprite(tmp3, SDL_Rect{ 0 * GHOST_TEXTURESIZE,0,GHOST_TEXTURESIZE,GHOST_TEXTURESIZE })
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
		animSet.add("up", up);
		animSet.add("down", down);
		animSet.add("left", left);
		animSet.add("right", right);
		animSet.add("upd", upd);
		animSet.add("downd", downd);
		animSet.add("leftd", leftd);
		animSet.add("rightd", rightd);
		animSet.add("afraid", afraid);
		animSet.add("afraidFlash", afraidFlash);

		m_registry.assign_or_replace<AnimationSet>(m_entity, animSet);
		m_registry.assign_or_replace<AnimationPtr>(m_entity, right);
	}

	Ghosts ghostID;
};