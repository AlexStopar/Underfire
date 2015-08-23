#include <SDL.h>
#include "gate.h"
#include "common.h"
#include "soundplayer.h"
#include "texturestorage.h"

bool isSpeedy = false;
void Gate::TriggerGate()
{
	if (isTriggered) return;
	isSpeedy = true;
	SoundPlayer::getInstance().playGateTrigger();
	isTriggered = true;
}

void Gate::move()
{
	if (pPosX < -GATE_WIDTH) return;
	if (pPosX > -GATE_WIDTH && pPosX < SCREEN_WIDTH) isOnScreen = true;
	else isOnScreen = false;
	if(!isSpeedy) pVelX = -pVelocity;
	else pVelX = -pVelocity * SPEED_MULTIPLIER;
	pPosX += pVelX;
	setHitbox();
}

void Gate::render(SDL_Renderer* renderer)
{
	TextureStorage::getInstance().gateTexture.render(pPosX, pPosY, renderer);
}

void Gate::setHitbox()
{
	gateBox.x = pPosX;
	gateBox.y = pPosY;
}