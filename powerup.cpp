#include <SDL.h>
#include <vector>
#include "powerup.h"
#include "common.h"

void PowerUp::setHitbox()
{
	pHitbox.x = pPosX;
	pHitbox.y = pPosY;
}

void PowerUp::move()
{
	if (pPosX > 0 && pPosX < SCREEN_WIDTH) isOnScreen = true;
	else isOnScreen = false;

	if (!isOnScreen)
	{
		if (frameCount % OFFSCREEN_MOVE_LIMIT == 0) pPosX--;
	}
	else{
		pVelX = -pVelocity;
		if (waveCount >= 0) pVelY = pVelocity;
		else pVelY = -pVelocity;
	}
	waveCount++;
	if (waveCount == MAX_WAVE_COUNT) waveCount = -MAX_WAVE_COUNT;
	if (!isSpeedy)
	{
		pPosX += pVelX;
		pPosY += pVelY;
	}
	else
	{
		pPosX += pVelX * SPEED_MULTIPLIER;
		pPosY += pVelY * SPEED_MULTIPLIER;
	}
	setHitbox();
}

void PowerUp::render(SDL_Renderer* renderer)
{
	if (!isOnScreen) return;
	SDL_Rect* currentClip = &powerUpClips[(int)powerType];
	TextureStorage::getInstance().powerUpTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer, currentClip);
}
