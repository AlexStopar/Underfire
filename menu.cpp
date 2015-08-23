#include "menu.h"
#include <SDL.h>
#include "soundplayer.h"
#include "textdraw.h"
#include "texturestorage.h"
#include "common.h"

void playMenu(SDL_Renderer* renderer, SDL_Window* window)
{
	int shipPosition = 0;
	bool isOnHighScoreMenu = false;
	int currentMenuSize = 0;
	isResetting = true;
	while (!isQuitting)
	{
		currentMenuSize = TextDraw::getInstance().menuTexture.size();
		SDL_Event e;
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				isQuitting = true;
				isResetting = false;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					toggleFullscreen(window);
					break;
				case SDLK_UP:
					if (shipPosition == 0) shipPosition = currentMenuSize - 1;
					else shipPosition--;
					SoundPlayer::getInstance().playMenuSwitch();
					break;
				case SDLK_DOWN:
					if (shipPosition == currentMenuSize - 1) shipPosition = 0;
					else shipPosition++;
					SoundPlayer::getInstance().playMenuSwitch();
					break;
				case SDLK_z:
					if (!isOnHighScoreMenu){
						switch (shipPosition)
						{
						case 0:
							TextDraw::getInstance().reset();
							return;
							break;
						case 1:
							isOnHighScoreMenu = true;
							break;
						case 2:
							isQuitting = true;
							isResetting = false;
							break;
						}
					}
					else isOnHighScoreMenu = false;
					SoundPlayer::getInstance().playMenuPick();
					break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xB2, 0x66, 0xFF);
		SDL_Rect background = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_RenderFillRect(renderer, &background);

		if (!isOnHighScoreMenu)
		{
			TextDraw::getInstance().renderMenuTitle(renderer);
			TextDraw::getInstance().renderMenu(renderer, shipPosition);
			TextDraw::getInstance().renderInstructions(renderer, false);
		}
		else
		{
			TextDraw::getInstance().renderHighScoreMenu(renderer);
			TextDraw::getInstance().renderInstructions(renderer, true);
		}
		SDL_RenderPresent(renderer);
	}

}