#pragma once

#include "Framework.h"
#include "TileObject.h"
#include <set>
#include <fstream>
#include <codecvt>
#include <sstream>
#include <string>

class Level
{
	public:
		Level(std::filesystem::path TileDir, std::filesystem::path LevelDir, int w = 20, int h = 20, Uint32 TextureAmount = 38)
		{
			for (Uint32 i = 0; i <= TextureAmount; i++)
			{
				std::filesystem::path tmp = TileDir / (std::to_string(i) + ".png");
				TileTexture.push_back(tmp);
			}
			for (size_t i = TileTexture.size() - 2; i < TileTexture.size(); i++)
				PelletTexturePath.insert(TileTexture[i]);
			std::filesystem::path LevelPath = LevelDir / "0.txt";
			std::fstream LevelInput;
			try
			{
				LevelInput = std::fstream(LevelPath.string(), std::fstream::in);
			}
			catch (const std::exception&)
			{
				std::cerr << "Error: [Level::LevelPath] Not Found" << std::endl;
				exit(404);
			}

			std::vector<std::vector<Uint32>> LevelCode;	// Use vector to initialize tiles
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
					tmp.push_back(TileTexture[LevelCode[i][j]]);
				}
				level.push_back(tmp);
				tmp.clear();
			}
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
					tmp.push_back(TileTexture[LevelCode[i][j]]);
				}
				level.push_back(tmp);
				tmp.clear();
			}
		}
		
		~Level()
		{
			delete pTileObject;
		}

		void init(entt::DefaultRegistry & registry, bloom::Game *& gameInstance)	// Creates a bunch of SmallWalls
		{

			for (Uint32 i = 0; i < level.size(); i++)
			{
				std::vector<TileObject*> a;
				for (Uint32 j = 0; j < level[i].size(); j++)
				{
					pTileObject = new TileObject(registry, gameInstance);
					a.push_back(pTileObject);
				}
				Tiles.push_back(a);
				a.clear();
			}
		}

		void generate(int w = 19, int h = 19)	// Uses Tiles to make the map (19 x 19 is the best size)
		{
			for(Uint32 i = 0; i < level.size(); i++)
				for (Uint32 j = 0; j < level[i].size(); j++)
				{
					Tiles[i][j]->init(level[i][j], SDL_Rect{ static_cast<int> (j * w),  static_cast<int> (i * h), w, h }, SDL_Rect{ 0, 0, 6, 6 });
					if (PelletTexturePath.count(level[i][j]) == 0)
						Tiles[i][j]->isWall();
					else
						Tiles[i][j]->isPellet();
				}
		}

		std::vector<std::vector<std::filesystem::path>> getLevelTexture()
		{
			return level;
		}
	protected:
		std::vector<std::vector<TileObject*>> Tiles;
		std::set<std::filesystem::path> PelletTexturePath;
		std::vector<std::filesystem::path> TileTexture;
		std::vector<std::vector<std::filesystem::path>> level;
		TileObject *pTileObject = NULL;
};