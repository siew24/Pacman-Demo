#pragma once
#include <filesystem>
#include "../getExePath.h"
const int TILETEXTURESIZE = 8;
const int PACMAN_TEXTURESIZE = 13;
const int GHOST_TEXTURESIZE = 14;
const int ENTITYSIZE = 12;
const int TILESIZE = 8;
const int ENTITYFRAMETIME = 150;
const int FRUIT_POS_X = 14;
const int FRUIT_POS_Y = 17;


const int WINDOW_WIDTH = 28 * TILESIZE;
const int WINDOW_HEIGHT = 36 * TILESIZE;

const SDL_Rect GHOST_BONUS_SIZE{ 0, 4, 12, 5 };
const SDL_Rect BONUS_SIZE{ -1, -1, 15, 6 };

const SDL_Rect GAMEAREA{ ((WINDOW_WIDTH / TILESIZE) - 28) / 2 * TILESIZE,3 * TILESIZE, 28 * TILESIZE, 33 * TILESIZE };

const std::filesystem::path ASSETPATH = std::filesystem::path(getExePath()) / L"data";