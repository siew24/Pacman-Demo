#pragma once

#include "Framework.h"
#include "InputDefinitions.h"

class InputManager
{
	public:
		void update()
		{
			m_keyDown = 0;
			m_keyUp = 0;
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				switch (event.type) {
				case SDL_KEYDOWN: {
					SDL_Keysym pressedKey = event.key.keysym;

					m_keyDown = pressedKey.scancode;
					break;
				}
				case SDL_KEYUP: {
					SDL_Keysym releasedKey = event.key.keysym;

					m_keyUp = releasedKey.scancode;

					break;
				}
				default:
					break;
				}
			}
		}
		bool isKeyPressed(KeyboardKey K)
		{
			if (m_keyDown == K)
				return true;
			return false;
		}
	private:
		int m_keyDown;
		int m_keyUp;
};