#include "lives.h"
#include <SDL.h>
#include "texturestorage.h"
#include "common.h"

Lives::Lives()
{
	livesLeft = 3;
}

void Lives::changeLifeCount(int amount)
{
	livesLeft -= amount;
}

int Lives::getLives()
{
	return livesLeft;
}

void Lives::setLives(int n)
{
	livesLeft = n;
}

void Lives::renderLives(SDL_Renderer* renderer)
{
	TextureStorage::getInstance().playerTexture.setHeight(LIFE_HEIGHT);
	TextureStorage::getInstance().playerTexture.setWidth(LIFE_WIDTH);
	int startingPosition = SCREEN_WIDTH - TextureStorage::getInstance().playerTexture.getWidth();
	for (int i = 0; i < livesLeft - 1; i++)
	{
		TextureStorage::getInstance().playerTexture.render(startingPosition, 
			LIFE_HEIGHT, renderer);
		startingPosition -= TextureStorage::getInstance().playerTexture.getWidth();
	}
}