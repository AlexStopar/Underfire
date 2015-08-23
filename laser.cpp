#include <SDL.h>
#include "laser.h"
#include "texturestorage.h"
#include "common.h"
#include "soundplayer.h"

const int PREP_TIME = 100;
void Laser::render(SDL_Renderer* renderer)
{
	TextureStorage::getInstance().laserTexture.setHeight(pHeight);
	TextureStorage::getInstance().laserTexture.setWidth(pWidth);
	TextureStorage::getInstance().laserTexture.render(pPosX, pPosY, renderer);
}

void Laser::move()
{
	if (!isOnScreen)
	{
		pPosX = -SCREEN_WIDTH;
		return;
	}
	if (isPreparing)
	{
		timer++; 
		if (timer >= PREP_TIME)
		{
			isPreparing = false;
			isRising = true;
			SoundPlayer::getInstance().playLaser();
		}
	}
	else if (isRising)
	{
		if (frameCount % 2 == 0)
		{
			pHeight += 2;
			pPosY--;
		}
		if (pHeight >= LASER_HEIGHT)
		{
			isRising = false;
			isFalling = true;
		}
	}
	else if (isFalling)
	{
		if (frameCount % 2 == 0)
		{
			pHeight -= 2;
			pPosY++;
		}
		if (pHeight <= 0)
		{
			isFalling = false;
			isOnScreen = false;
		}
	}
	setHitbox();
}

void Laser::setHitbox()
{
	laserBox.x = pPosX;
	laserBox.y = pPosY;
	laserBox.h = pHeight;
}