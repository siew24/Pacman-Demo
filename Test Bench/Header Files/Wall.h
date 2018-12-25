#pragma once

#include "Framework.h"
#include "SmallWall.h"
#include <fstream>
#include <codecvt>
#include <string>

class Wall
{
	public:
		Wall(entt::DefaultRegistry &a, bloom::Game *& gameinstance, std::filesystem::path WallDir, std::filesystem::path LevelDir, int w, int h, Uint32 WallSize = 42)
		{
			init(WallDir, LevelDir, WallSize);
			for (Uint32 i = 0; i < Level.size(); i++)
				for (Uint32 j = 0; j < Level[i].size(); j++)
				{
					SmallWall tmp = SmallWall(a, gameinstance);
					tmp.init(Level[i][j], { static_cast<int> (i * 10), static_cast<int> (j * 10), w, h }, {101, 23, 7, 7});
				}
		}
		void init(std::filesystem::path WallDir, std::filesystem::path LevelDir, Uint32 WallSize)
		{
			for (Uint32 i = 1; i <= 42; i++)
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
			char Temp;
			while (LevelInput >> Temp)
			{
				if (Temp == ',')
				{
					LevelCode.push_back(LevelCode1);
					LevelCode1.clear();
				}
				else
					LevelCode1.push_back(Temp - 48);
			}
			for (Uint32 i = 0; i < LevelCode.size(); i++)
			{
				std::vector<std::filesystem::path> tmp;
				for (Uint32 j = 0; j < LevelCode[i].size(); j++)
				{
					tmp.push_back(Walls[LevelCode[i][j] - 1]);
				}
				Level.push_back(tmp);
				tmp.clear();
			}
		}
	protected:
		std::vector<std::filesystem::path> Walls;
		std::vector<std::vector<std::filesystem::path>> Level;
};