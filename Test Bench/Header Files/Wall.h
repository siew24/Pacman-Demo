#pragma once

#include "Framework.h"
#include "SmallWall.h"
#include <fstream>
#include <codecvt>
#include <sstream>
#include <string>

class Wall
{
	public:
		Wall(std::filesystem::path WallDir, std::filesystem::path LevelDir, int w = 20, int h = 20, Uint32 WallSize = 38)
		{
			for (Uint32 i = 0; i <= WallSize; i++)
			{
				std::filesystem::path tmp = WallDir / (std::to_string(i) + ".png");
				Walls.push_back(tmp);
			}

			std::filesystem::path LevelPath = LevelDir / "0.txt";
			std::fstream LevelInput;
			try
			{
				LevelInput = std::fstream(LevelPath.string(), std::fstream::in);
			}
			catch (const std::exception&)
			{
				std::cerr << "Error: [Wall::LevelPath] Not Found" << std::endl;
				exit(404);
			}

			std::vector<std::vector<Uint32>> LevelCode;
			std::vector<Uint32> LevelCode1;
			std::string Temp;
			while (std::getline(LevelInput, Temp))
			{
				std::stringstream ss;
				ss << Temp;
				std::string temp;
				while (!ss.eof())
				{
					ss >> temp;
					int a;
					std::stringstream(temp) >> a;
					LevelCode1.push_back(a);
				}
				LevelCode.push_back(LevelCode1);
				LevelCode1.clear();
			}
			for (Uint32 i = 0; i < LevelCode.size(); i++)
			{
				std::vector<std::filesystem::path> tmp;
				for (Uint32 j = 0; j < LevelCode[i].size(); j++)
				{
					tmp.push_back(Walls[LevelCode[i][j]]);
				}
				Level.push_back(tmp);
				tmp.clear();
			}
		}
		~Wall()
		{
			delete pSmallWall;
		}

		void init(entt::DefaultRegistry & registry, bloom::Game *& gameInstance)	// Creates a bunch of SmallWalls
		{

			for (Uint32 i = 0; i < Level.size(); i++)
			{
				std::vector<SmallWall*> a;
				for (Uint32 j = 0; j < Level[i].size(); j++)
				{
					pSmallWall = new SmallWall(registry, gameInstance);
					a.push_back(pSmallWall);
				}
				Tiles.push_back(a);
				a.clear();
			}
		}

		void generate(int w = 19, int h = 19)	// Uses Tiles to make the map (19 x 19 is the best size)
		{
			for(Uint32 i = 0; i < Level.size(); i++)
				for (Uint32 j = 0; j < Level[i].size(); j++)
					Tiles[i][j]->init(Level[i][j], SDL_Rect{ static_cast<int> (j * w),  static_cast<int> (i * h), w, h }, SDL_Rect{ 0, 0, 6, 6 });
		}

		std::vector<std::vector<std::filesystem::path>> getLevelTexture()
		{
			return Level;
		}
	protected:
		std::vector<std::vector<SmallWall*>> Tiles;
		std::vector<std::filesystem::path> Walls;
		std::vector<std::vector<std::filesystem::path>> Level;
		SmallWall *pSmallWall = NULL;
};