#include <SDL.h>
#include <random>
#include "bullet.h"
#include "level.h"
#include "boss.h"
#include "common.h"
#include "soundplayer.h"
#include "texturestorage.h"

const int MAX_LEAN_TIME = 10;
const int SHOOT_LIMIT = 20;
void Bullet::setHitbox()
{
	pHitbox.x = pPosX;
	pHitbox.y = pPosY;
}

void Bullet::move()
{
	if (pPosX < 0 || pPosY < 0 || pPosY > SCREEN_HEIGHT)
	{
		pPosX = -SCREEN_WIDTH;
		setHitbox();
		return;
	}
	if (isGrowing)
	{
		pWidth++;
		pHeight++;
	}
	if (pPosX > SCREEN_WIDTH && !isEnemy) pPosX = -SCREEN_WIDTH;
	if (bulletMovementType != MoveType::STRAIGHTDOWN && bulletMovementType != MoveType::STRAIGHTUP)
	{
		if (!isEnemy) pVelX = pVelocity;
		else pVelX = -pVelocity;
	}
	if (frameCount % SHOOT_LIMIT == 0 && isShooting) shootBullet();
	switch (bulletMovementType)
	{
	case MoveType::FORWARD:	
		if (isEnemy) pVelX = -pVelocity;
		break;
	case MoveType::UP:
		pVelY = pVelocity;
		angle = 45;
		break;
	case MoveType::DOWN:
		pVelY = -pVelocity;
		angle = -45;
		break;
	case MoveType::STRAIGHTDOWN:
		pVelY = pVelocity;
		break;
	case MoveType::STRAIGHTUP:
		pVelY = -pVelocity;
		break;
	case MoveType::DIAGONAL:
		pVelX = -pVelocity;
		if (isAbovePlayer) pVelY = pVelocity/2;
		else pVelY = -pVelocity/2;
		break;
	case MoveType::NEGDIAGONAL:
		pVelX = pVelocity;
		if (isAbovePlayer) pVelY = pVelocity / 2;
		else pVelY = -pVelocity / 2;
		break;
	}
	if (leanTime > MAX_LEAN_TIME && bulletMovementType != MoveType::DIAGONAL && !isEnemy) {
		pVelY = 0;
		angle = 0;
	}
	if (!isSpeedy || !isEnemy || isBoss)
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
	leanTime++;
	if (pPosX < 0 || pPosX > SCREEN_WIDTH || pPosY < 0 || pPosY > SCREEN_HEIGHT)
	{
		isOnScreen = false;
	}
}

void Bullet::render(SDL_Renderer* renderer)
{
	TextureStorage::getInstance().enemyBulletTexture.setHeight(pHeight);
	TextureStorage::getInstance().enemyBulletTexture.setWidth(pWidth);
	setHitbox();
	pHitbox.radius = pWidth / 2;
	
	if (pPosX < 0 || pPosY < 0 || pPosY > SCREEN_HEIGHT) return;
	if(!isEnemy)TextureStorage::getInstance().bulletTexture.render(pPosX - pHitbox.radius,
		pPosY - pHitbox.radius, renderer, NULL, angle);
	else TextureStorage::getInstance().enemyBulletTexture.render(pPosX - pHitbox.radius,
		pPosY - pHitbox.radius, renderer, NULL, angle);
}

void Bullet::shootBullet()
{
	SoundPlayer::getInstance().playShoot(true);
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> randomSet(0, 1);
	tempBullets.push_back(Bullet(pPosX - pHitbox.radius, pPosY, ENEMY_BULLET_WIDTH,
		ENEMY_BULLET_HEIGHT, ENEMY_BULLET_VEL, MoveType::DIAGONAL, true, !!randomSet(eng)));
}