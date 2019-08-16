#pragma once
#include <string>
#include "Game.h"
#include "Graphics/SpriteText.h"
#include "Graphics/Font.h"
#include "../Configs.h"
#include "Input/InputManager.h"
#include "../Tile.h"
#include "TilePicker.h"
#include "SpecialPicker.h"
#include "FileMenu.h"


class LevelCreator {
	using InputManager = bloom::input::InputManager;
	using KeyboardKeys = bloom::input::KeyboardKey;

public:
	LevelCreator( bloom::Game*& gameInstance, bloom::graphics::FontPtr guiFont ) :
		m_gameInstance( gameInstance ), m_renderer( gameInstance->_getRenderer() ),
		tilePicker( gameInstance, guiFont ),
		specialPicker( gameInstance, guiFont ),
		menu( gameInstance, guiFont )
	{
		if ( m_levelTex )
			SDL_DestroyTexture( m_levelTex );
		if ( m_specialTex )
			SDL_DestroyTexture( m_specialTex );

		m_levelTex = SDL_CreateTexture( m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28 * TILESIZE) + 2, (31 * TILESIZE) + 2 );
		m_specialTex = SDL_CreateTexture( m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (28 * TILESIZE) + 2, (31 * TILESIZE) + 2 );
		SDL_SetTextureBlendMode( m_specialTex, SDL_BLENDMODE_BLEND );

		guiElems.emplace_back( std::make_shared<bloom::graphics::SpriteText>( m_renderer, guiFont, "Level Creator" ) );
		guiElems.emplace_back( std::make_shared<bloom::graphics::SpriteText>( m_renderer, guiFont, "Selected tile: " ) );
		guiElems.emplace_back( std::make_shared<bloom::graphics::SpriteText>( m_renderer, guiFont, "[T]iles" ) );
		guiElems.emplace_back( std::make_shared<bloom::graphics::SpriteText>( m_renderer, guiFont, "[S]pecials" ) );
		guiElems.emplace_back( std::make_shared<bloom::graphics::SpriteText>( m_renderer, guiFont, "[F]ile" ) );
		//guiElems.emplace_back(std::make_shared<bloom::graphics::SpriteText>(m_renderer, guiFont, "[L]oad"));

		updateTilemapTexture();

		std::filesystem::path appData;
		{
			char* pValue;
			size_t len;
			errno_t err = _dupenv_s( &pValue, &len, "APPDATA" );
			appData = std::filesystem::path( pValue );
		}
		saveDir = appData / "BF Pacman" / "Custom Levels";
		if ( !std::filesystem::exists( saveDir ) )
			std::filesystem::create_directory( saveDir );
	}
	~LevelCreator() {
		SDL_DestroyTexture( m_levelTex );
	}

	void draw() {
		SDL_RenderClear( m_renderer );
		auto tmp = GAMEAREA;
		tmp.h -= 14, tmp.w += 2, tmp.x -= 1, tmp.y -= 1;
		SDL_RenderCopyEx( m_renderer, m_levelTex, nullptr, &tmp, 0.0, nullptr, SDL_FLIP_NONE );
		if ( currentMode == 1 )
			SDL_RenderCopyEx( m_renderer, m_specialTex, nullptr, &tmp, 0.0, nullptr, SDL_FLIP_NONE );
		guiElems[0]->render( std::nullopt, SDL_Point{ 120 - (guiElems[0]->getTextWidth() / 2), 8 } );
		//guiElems[1]->render(std::nullopt, SDL_Point{ 8, 16 });
		/*{
			SDL_Rect tileLocation{ 8 + 15 * 8, 16,8,8 };
			m_gameInstance->textures.load(ASSETPATH / "Assets" / "Tile" / (std::to_string(m_selectedTileID) + ".png"))->render(std::nullopt, tileLocation);
		}*/
		guiElems[2]->render( std::nullopt, SDL_Point{ 8, 36 * TILESIZE } );
		guiElems[3]->render( std::nullopt, SDL_Point{ 80, 36 * TILESIZE } );
		guiElems[4]->render( std::nullopt, SDL_Point{ 168, 36 * TILESIZE } );
		//guiElems[5]->render(std::nullopt, SDL_Point{ 210, 36 * TILESIZE });

		SDL_Rect offset = { 0,0,8,8 };

		if ( currentMode == 1 ) {
			auto& tmp = std::get<1>( specialsInfo[m_selected] );
			offset = SDL_Rect{ tmp.x, tmp.y, tmp.w, tmp.h };
		}
		SDL_Rect selectorPos = { tmp.x + (m_currentTile.x * 8) + offset.x, tmp.y + (m_currentTile.y * 8) + offset.y, 3,3 };
		SDL_Rect selectedTextDst{ selectorPos.x + 1, selectorPos.y + 1, offset.w, offset.h };
		SDL_RenderCopyEx( m_renderer, m_selectedTexture, nullptr, &selectedTextDst, 0.0, nullptr, SDL_FLIP_NONE );

		std::string append = (justPlaced > 0.0 ? " G" : "");

		m_gameInstance->textures.load( ASSETPATH / "Assets" / "LevelCreator" / std::string( "TL Corner" + append + ".png" ) )->render( std::nullopt, selectorPos );
		selectorPos.x += offset.w - 1;
		m_gameInstance->textures.load( ASSETPATH / "Assets" / "LevelCreator" / std::string( "TR Corner" + append + ".png" ) )->render( std::nullopt, selectorPos );
		selectorPos.y += offset.h - 1;
		m_gameInstance->textures.load( ASSETPATH / "Assets" / "LevelCreator" / std::string( "BR Corner" + append + ".png" ) )->render( std::nullopt, selectorPos );
		selectorPos.x -= offset.w - 1;
		m_gameInstance->textures.load( ASSETPATH / "Assets" / "LevelCreator" / std::string( "BL Corner" + append + ".png" ) )->render( std::nullopt, selectorPos );
	}
	void update( double dt ) {
		if ( !m_selectedTexture )
			updateSelectedTexture();

		auto& keyboard = m_gameInstance->input.keyboard;
		m_inputInterval -= dt;
		bool haveInput = false;
		justPlaced -= dt;

		if ( keyboard.wasDown( KeyboardKeys::KEY_T ) ) {
			auto tmp = tilePicker.openSelector();
			if ( !tilePicker.cancelled ) {
				currentMode = 0;
				m_selected = tmp;
				updateSelectedTexture();
			}
			dontEnter = true;
			justPlaced = 0.0;
		}
		else if ( keyboard.wasDown( KeyboardKeys::KEY_S ) ) {
			auto tmp = specialPicker.openSelector();
			if ( !specialPicker.cancelled ) {
				currentMode = 1;
				m_selected = tmp;
				updateSelectedTexture();
			}
			dontEnter = true;
			justPlaced = 0.0;
		}
		else if ( keyboard.wasDown( KeyboardKeys::KEY_F ) ) {
			auto tmp = menu.openMenu();
			if ( tmp.first == "open" )
				load( tmp.second );
			else if ( tmp.first == "save" )
				save( tmp.second );
			justPlaced = 0.0;
			dontEnter = true;
		}
		else if ( keyboard.wasDown( KeyboardKeys::KEY_Q ) ) {
			selected = 1;
		}

		if ( keyboard.isPressed( KeyboardKeys::KEY_UP ) ) {
			if ( m_inputInterval <= 0.0 ) --m_currentTile.y;
			haveInput = true;
			justPlaced = 0.0;
		}
		if ( keyboard.isPressed( KeyboardKeys::KEY_DOWN ) ) {
			if ( m_inputInterval <= 0.0 ) ++m_currentTile.y;
			haveInput = true;
			justPlaced = 0.0;
		}
		if ( keyboard.isPressed( KeyboardKeys::KEY_LEFT ) ) {
			if ( m_inputInterval <= 0.0 ) --m_currentTile.x;
			haveInput = true;
			justPlaced = 0.0;
		}
		if ( keyboard.isPressed( KeyboardKeys::KEY_RIGHT ) ) {
			if ( m_inputInterval <= 0.0 ) ++m_currentTile.x;
			haveInput = true;
			justPlaced = 0.0;
		}

		// Make sure the reticle is still within bounds.
		if ( m_currentTile.y < 0 ) m_currentTile.y = 30;
		if ( m_currentTile.y > 30 ) m_currentTile.y = 0;
		if ( m_currentTile.x < 0 ) m_currentTile.x = 27;
		if ( m_currentTile.x > 27 ) m_currentTile.x = 0;


		if ( keyboard.isPressed( KeyboardKeys::KEY_RETURN ) || keyboard.isPressed( KeyboardKeys::KEY_KEYPAD_ENTER ) ) {
			bool changed = false;
			if ( !dontEnter ) {
				justPlaced = 500.0;
				if ( currentMode == 0 ) {
					if ( m_tileMap[m_currentTile.x][m_currentTile.y] != m_selected ) {
						m_tileMap[m_currentTile.x][m_currentTile.y] = m_selected;
						updateTilemapTexture();
					}
				}
				else if ( currentMode == 1 ) {
					if ( m_specialsMap[m_currentTile.x][m_currentTile.y] != m_selected ) {
						m_specialsMap[m_currentTile.x][m_currentTile.y] = m_selected;
						updateSpecialTexture();
					}
				}
			}
		}


		if ( haveInput )
			if ( constant )
				m_inputInterval = m_inputInterval <= 0.0 ? 33.3333 : m_inputInterval;
			else
				constant = true, m_inputInterval = m_inputInterval <= 0.0 ? 500.0 : m_inputInterval;
		else
			constant = false, m_inputInterval = 0.0;

		if ( !keyboard.isPressed( KeyboardKeys::KEY_RETURN ) && !keyboard.isPressed( KeyboardKeys::KEY_KEYPAD_ENTER ) && dontEnter ) {
			dontEnter = false;
		}
	}
	int selected = -1;

private:
	void updateTilemapTexture() {
		auto origColor = m_gameInstance->getColor();
		m_gameInstance->setColor(SDL_Color{ 255, 0, 0, 0 });
		SDL_SetRenderTarget( m_renderer, m_levelTex );
		SDL_RenderClear( m_renderer );
		m_gameInstance->setColor( origColor );

		for ( int x = 0; x < 28; ++x ) {
			for ( int y = 0; y < 31; ++y ) {
				SDL_Rect tileLocation{ x * 8 + 1, y * 8 + 1,8,8 };
				m_gameInstance->textures.load( ASSETPATH / "Assets" / "Tile" / (std::to_string( m_tileMap[x][y] ) + ".png") )->render( std::nullopt, tileLocation );
			}
		}
		SDL_SetRenderTarget( m_renderer, nullptr );
	}
	void updateSpecialTexture() {
		auto origColor = m_gameInstance->getColor();
		m_gameInstance->setColor(SDL_Color{ 0, 0, 0, 0 });
		SDL_SetRenderTarget( m_renderer, m_specialTex );
		SDL_RenderClear( m_renderer );
		m_gameInstance->setColor( origColor );

		for ( int x = 0; x < 28; ++x ) {
			for ( int y = 0; y < 31; ++y ) {
				if ( m_specialsMap[x][y] > 0 ) {
					auto& rectDet = std::get<1>( specialsInfo[m_specialsMap[x][y]] );
					SDL_Rect tileLocation{ x * 8 + 1 + rectDet.x, y * 8 + 1 + rectDet.y,rectDet.w,rectDet.h };
					m_gameInstance->textures.load( std::get<0>( specialsInfo[m_specialsMap[x][y]] ) )->render( std::nullopt, tileLocation );
				}
			}
		}
		SDL_SetRenderTarget( m_renderer, nullptr );
	}
	void updateSelectedTexture() {
		if ( m_selectedTexture )
			SDL_DestroyTexture( m_selectedTexture );
		auto rectDet = SDL_Rect{ 0,0,8,8 };
		if ( currentMode == 1 ) rectDet = std::get<1>( specialsInfo[m_selected] );

		m_selectedTexture = SDL_CreateTexture( m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rectDet.w, rectDet.w );
		//SDL_SetTextureBlendMode(m_selectedTexture, SDL_BLENDMODE_BLEND);

		auto origColor = m_gameInstance->getColor();
		SDL_SetRenderTarget( m_renderer, m_selectedTexture );
		if ( currentMode == 0 ) {
			SDL_Rect tileLocation{ 0, 0,8,8 };
			m_gameInstance->textures.load( ASSETPATH / "Assets" / "Tile" / (std::to_string( m_selected ) + ".png") )->render( std::nullopt, tileLocation );
		}
		else if ( currentMode == 1 ) {
			SDL_Rect tileLocation{ 0,0,rectDet.w,rectDet.h };
			m_gameInstance->textures.load( std::get<0>( specialsInfo[m_selected] ) )->render( std::nullopt, tileLocation );
		}
		//m_gameInstance->textures.load(ASSETPATH / "Assets" / "LevelCreator" / "Tint.png")->render(std::nullopt, tileLocation);
		SDL_SetRenderTarget( m_renderer, nullptr );
	}
	void load( std::string filename ) {
		std::filesystem::path file = saveDir / filename;

		if ( std::filesystem::exists( file ) && filename != "" ) {
			std::ifstream fileData( file );
			for ( int y = 0; y < 31; y++ )
				for ( int x = 0; x < 28; x++ )
					fileData >> m_tileMap[x][y];

			for ( int y = 0; y < 31; y++ )
				for ( int x = 0; x < 28; x++ )
					fileData >> m_specialsMap[x][y];
		}
		else {
			for ( int y = 0; y < 31; y++ )
				for ( int x = 0; x < 28; x++ )
					m_tileMap[x][y] = 0;

			for ( int y = 0; y < 31; y++ )
				for ( int x = 0; x < 28; x++ )
					m_specialsMap[x][y] = 0;
		}
		updateTilemapTexture();
		updateSpecialTexture();
	}
	void save( std::string filename ) {
		std::filesystem::path file = saveDir / filename;
		if ( filename != "" ) {
			std::ofstream fileData( file );
			for ( int y = 0; y < 31; y++ ) {
				for ( int x = 0; x < 28; x++ )
					fileData << m_tileMap[x][y] << " ";
				fileData << std::endl;
			}
			fileData << std::endl;
			for ( int y = 0; y < 31; y++ ) {
				for ( int x = 0; x < 28; x++ )
					fileData << m_specialsMap[x][y] << " ";
				fileData << std::endl;
			}
		}
	}

	bloom::Game*& m_gameInstance;
	SDL_Renderer* m_renderer;
	SDL_Texture* m_levelTex = nullptr;
	SDL_Texture* m_specialTex = nullptr;

	// GUI Text
	std::vector<std::shared_ptr<bloom::graphics::SpriteText>> guiElems;

	const int m_levelTexOffset = 2;

	Tile m_currentTile{ 0,0 };
	int m_selected = 1;
	SDL_Texture* m_selectedTexture;
	int currentMode = 0;
	std::array<std::array<int, 31>, 28> m_tileMap;
	std::array<std::array<int, 31>, 28> m_specialsMap;
	double m_inputInterval = 0.0;
	bool constant = false;
	double dontEnter = true;
	double justPlaced = 0.0;

	TilePicker tilePicker;
	SpecialPicker specialPicker;
	FileMenu menu;

	const std::filesystem::path assetPath = ASSETPATH / "Assets";
	const std::vector<std::tuple<std::filesystem::path, SDL_Rect>> specialsInfo{
		{assetPath / "LevelCreator" / "Icons" / "0.png",SDL_Rect {0,0,8,8}}, // Blank
		{assetPath / "LevelCreator" / "Icons" / "Pacman.png",SDL_Rect {-2 - (PACMAN_TEXTURESIZE - TILESIZE) / 2,-(PACMAN_TEXTURESIZE - TILESIZE) / 2,13,13}}, // Pacman
		{assetPath / "LevelCreator" / "Icons" / "Blinky.png",SDL_Rect {-(GHOST_TEXTURESIZE - TILESIZE) / 2,-(GHOST_TEXTURESIZE - TILESIZE) / 2,14,14}}, // Blinky
		{assetPath / "LevelCreator" / "Icons" / "Pinky.png",SDL_Rect {-(GHOST_TEXTURESIZE - TILESIZE) / 2,-(GHOST_TEXTURESIZE - TILESIZE) / 2,14,14}}, // Pinky
		{assetPath / "LevelCreator" / "Icons" / "Bashful.png",SDL_Rect {-(GHOST_TEXTURESIZE - TILESIZE) / 2,-(GHOST_TEXTURESIZE - TILESIZE) / 2,14,14}}, // Bashful
		{assetPath / "LevelCreator" / "Icons" / "Pokey.png",SDL_Rect {-(GHOST_TEXTURESIZE - TILESIZE) / 2,-(GHOST_TEXTURESIZE - TILESIZE) / 2,14,14}}, // Pokey
		{assetPath / "LevelCreator" / "Icons" / "Bonus.png",SDL_Rect {-(12 - TILESIZE) / 2,-(12 - TILESIZE) / 2,12,12}}, // Bonus
		{assetPath / "LevelCreator" / "Icons" / "No Up.png",SDL_Rect {0 ,0,8,8}}, // No-Up zones
		{assetPath / "LevelCreator" / "Icons" / "Half-Speed.png",SDL_Rect {0 ,0,8,8}}, // Halfspeed zone
		{assetPath / "LevelCreator" / "Icons" / "GhostHome.png",SDL_Rect {0 ,0,8,8}}, // Ghost Home
		{assetPath / "LevelCreator" / "Icons" / "Block.png",SDL_Rect {0 ,0,8,8}} // Ghost Home
	};

	std::filesystem::path saveDir;
};