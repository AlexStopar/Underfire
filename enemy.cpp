#include <SDL.h>
#include <vector>
#include <cmath>
#include <random>
#include "enemy.h"
#include "level.h"
#include "texturestorage.h"
#include "soundplayer.h"
#include "textdraw.h"
#include "common.h"
#include "bullet.h"

const int VERTICAL_MOVE_LIMIT = 15;
const int HORIZONTAL_MOVE_LIMIT = 7;
const int MEDIUM_MOVE_CHANGE_LIMIT = 30;
const int LEVEL_TWO_PAUSE_DELAY = 15;
const int ANGLE_CHANGE_LIMIT = 3;

const int EASY_HP = 1;
const int EASY2_HP = 2;
const int MEDIUM_HP = 2;
const int MEDIUM2_HP = 3;

void Enemy::move(std::vector<Bullet>& bullets, bool isAbovePlayer)
{
	if (isDead)
	{
		setHitbox();
		return;
	}
	if (pPosX > 0 && pPosX < SCREEN_WIDTH)
	{
		if (!isOnScreen)
		{
			SoundPlayer::getInstance().playShipMove(true);
			assignHP();
		}
		isOnScreen = true;
	}
	else isOnScreen = false;

	if (!isOnScreen)
	{
		if (frameCount % OFFSCREEN_MOVE_LIMIT == 0) pPosX--;
		setHitbox();
		return;
	}

	if (eType == EnemyType::EASY && isShooting && currentLevel == Level::CITY_LIGHTS) shootBullet(isAbovePlayer);
	else if (frameCount % VERTICAL_MOVE_LIMIT == 0 && (currentLevel != Level::CITY_LIGHTS || eType == EnemyType::MEDIUM)) shootBullet(isAbovePlayer);
	if (frameCount % LEVEL_TWO_PAUSE_DELAY == 0)
	{
		if (isShooting) isShooting = false;
		else isShooting = true;
	}

	if (frameCount % HORIZONTAL_MOVE_LIMIT == 0 && eType == EnemyType::EASY) pVelX--;
	if (frameCount % MEDIUM_MOVE_CHANGE_LIMIT == 0 && eType == EnemyType::MEDIUM && currentLevel == Level::BATTLE_RUINS)
	{
		if (mType == EnemyMoveType::STRAIGHT)
		{
			if (pPosY > SCREEN_HEIGHT / 2) mType = EnemyMoveType::CURVEUP;
			else mType = EnemyMoveType::CURVEDOWN;
		}
		else mType = EnemyMoveType::STRAIGHT;
	}

	switch (mType)
	{
	case EnemyMoveType::STRAIGHT:
		if (eType == EnemyType::MEDIUM) pVelX = -ENEMY_VEL;
		pVelY = 0;
		break;
	case EnemyMoveType::CURVEDOWN:
		if (frameCount % VERTICAL_MOVE_LIMIT == 0)
		{
			if (eType == EnemyType::EASY) pVelY++;
			else
			{
				pVelX = 0;
				pVelY = ENEMY_VEL;
			}
			if (pVelY == 4 && currentLevel == Level::SPEED_BRIDGE 
				&& eType == EnemyType::EASY) mType = EnemyMoveType::CURVEUP;
		}
		break;
	case EnemyMoveType::CURVEUP:
		if (frameCount % VERTICAL_MOVE_LIMIT == 0)
		{
			if (eType == EnemyType::EASY) pVelY--;
			else
			{
				pVelX = 0;
				pVelY = -ENEMY_VEL;
			}
			
		}
		break;
	}
	if (eType == EnemyType::MEDIUM && currentLevel != Level::BATTLE_RUINS && frameCount % 2 != 0) return;
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

	for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
		if (checkCollision(pHitbox, it->getHitbox()) && !(it->isEnemy))
		{
			it->setX(2 * SCREEN_WIDTH);
			it->setHitbox();
			hp--;
		
			if (hp <= 0 && !isDead)
			{
				isDead = true;
				frame = 0;
				if(eType == EnemyType::EASY) score += ENEMY_EASY_SCORE;
				else score += ENEMY_MEDIUM_SCORE;
				SoundPlayer::getInstance().playDeath(true);
			}
			else SoundPlayer::getInstance().playHit(true);
		}
	}
}

void Enemy::setHitbox()
{
	if (isDead) pHitbox.x = -SCREEN_WIDTH;
	else
	{
		pHitbox.x = pPosX;
		pHitbox.y = pPosY;
	}
}

void Enemy::render(SDL_Renderer* renderer)
{
	if (!isOnScreen) return;
	if (!isDead)
	{
		if (eType == EnemyType::EASY)
		{
			if (angle != 0 && frameCount % ANGLE_CHANGE_LIMIT == 0)
			{
				if (mType == EnemyMoveType::CURVEDOWN && currentLevel == Level::CITY_LIGHTS) angle++;
				else if (currentLevel == Level::CITY_LIGHTS) angle--;
				else if (currentLevel == Level::SPEED_BRIDGE) angle += setAngle;
			}
			TextureStorage::getInstance().enemyEasyTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer, NULL, angle);
		}
		else if(currentLevel != Level::SPEED_BRIDGE) TextureStorage::getInstance().enemyMediumTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer);
		else
		{
			SDL_Rect* currentClip = &levelThreeEnemyClips[frame];
			TextureStorage::getInstance().enemyMediumTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer, currentClip);
			if (frameCount % 10 == 0)
			{
				frame = (frame + 1) % LEVEL_3_ENEMY_CLIPS;
			}
		}
	}
	else if (frame != EXPLOSION_FRAMES)
	{
		SDL_Rect* currentClip = &explosionClips[frame];
		TextureStorage::getInstance().explosionTexture.render(pPosX, pPosY, renderer, currentClip);
		frame++;
	}
}

void Enemy::assignHP()
{
	switch (eType)
	{
	case EnemyType::EASY:
		if (currentLevel != Level::CITY_LIGHTS) hp = EASY_HP;
		else hp = EASY2_HP;
		break;
	case EnemyType::MEDIUM:
		if (currentLevel == Level::BATTLE_RUINS) hp = MEDIUM_HP;
		else hp = MEDIUM2_HP;
		break;
	}
}

void Enemy::shootBullet(bool isAbovePlayer)
{	
	SoundPlayer::getInstance().playShoot(true);
	if (eType == EnemyType::EASY)
	{
		if (mType != EnemyMoveType::STRAIGHT) bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DIAGONAL, true, isAbovePlayer, resolveGrowingBullets(true)));
		else bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, isAbovePlayer, resolveGrowingBullets(true)));
	}
	else
	{
		if (currentLevel != Level::SPEED_BRIDGE)
		{
			if (currentLevel == Level::BATTLE_RUINS) bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, isAbovePlayer, false));
			bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::UP, true, isAbovePlayer, resolveGrowingBullets(false)));
			bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DOWN, true, isAbovePlayer, resolveGrowingBullets(false)));
		}
		else
		{
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> randomSet(0, 1);
			bullets.push_back(Bullet(pPosX, pPosY, ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DIAGONAL, true, !!randomSet(eng), false, true));
		}
	}
}