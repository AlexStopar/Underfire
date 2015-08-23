#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "level.h"
#include "highscore.h"
#include "menu.h"
#include "common.h"

bool init();

void close();

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

Level currentLevel = Level::BATTLE_RUINS;

int setLives = 3;
int bulletPower = 1;

bool isContinuing = false;

bool init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) printf("Warning: Linear texture filtering not enabled!");

		gWindow = SDL_CreateWindow("Under Fire - By Alex Stopar", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			return false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					return false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					return false;
				}
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					return false;
				}
			}
		}
	}
	return true;
}

void close()
{
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;
	IMG_Quit();
	SDL_Quit();
	Mix_Quit();
	TTF_Quit();
}

int main(int argc, char* args[])
{
	if (!init()) printf("Failed to initialize!\n");
	else
	{
		loadMedia(gRenderer, false);
		HighScore::getInstance().ReadScore();
		while (!isQuitting)
		{
			playMenu(gRenderer, gWindow);
			while (isResetting)
			{
				if (isContinuing)
				{
					if(!isWon) currentLevel = (Level) (((int)currentLevel + 1) % (int)Level::TOTAL);
					else currentLevel = Level::BATTLE_RUINS;
					loadMedia(gRenderer, true);
					isContinuing = false;
				}
				playLevel(gRenderer, gWindow);
			}
			currentLevel = Level::BATTLE_RUINS;
			loadMedia(gRenderer, true);
		}
		HighScore::getInstance().WriteScore();
	}
	close();
	return 0;
}