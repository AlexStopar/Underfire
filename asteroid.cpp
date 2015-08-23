#include <SDL.h>
#include <vector>
#include "bullet.h"
#include "level.h"
#include "common.h"
#include "soundplayer.h"
#include "asteroid.h"

const int SHOOT_LIMIT = 60;
const int LEVEL_TWO_DIRECTION_LIMIT = 40;
void Asteroid::move(std::vector<Bullet>& bullets)
{
	if (pPosX < -pWidth || pPosY < -pHeight || pPosY > SCREEN_HEIGHT) isDead = true;
	if (isDead)
	{
		setHitbox();
		return;
	}
	if (pPosX > 0 && pPosX < SCREEN_WIDTH) isOnScreen = true;
	else isOnScreen = false;
	if (pPosX < -pWidth) pPosX = -SCREEN_WIDTH;
	if (!isOnScreen)
	{
		if (frameCount % OFFSCREEN_MOVE_LIMIT == 0) pPosX--;
		setHitbox();
		return;
	}
	if (frameCount % SHOOT_LIMIT == 0 && currentLevel == Level::SPEED_BRIDGE) shootBullet();
	switch (rockDirection)
	{
	case Direction::W:
		pVelY = 0;
		pVelX = -pVelocity;
		break;
	case Direction::NW:
		pVelY = -pVelocity;
		pVelX = -pVelocity * 2;
		break;
	case Direction::SW:
		pVelY = pVelocity;
		pVelX = -pVelocity * 2;
		break;
	case Direction::S:
		pVelY = pVelocity;
		pVelX = 0;
		break;
	case Direction::N:
		pVelY = -pVelocity;
		pVelX = 0;
		break;
	}
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
			hp--;
			it->setX(2 * SCREEN_WIDTH);
			it->setHitbox();
			if (hp <= 0 && !isDead)
			{
				SoundPlayer::getInstance().playDeath(true);
				die();
			}
			else SoundPlayer::getInstance().playHit(true);
		}
	}
	if (currentLevel != Level::BATTLE_RUINS && initDirection == Direction::W)
	{
		if (frameCount % LEVEL_TWO_DIRECTION_LIMIT == 0)
		{
			switch (rockDirection)
			{
			case Direction::W:
				rockDirection = Direction::NW;
				break;
			case Direction::NW:
				rockDirection = Direction::SW;
				break;
			case Direction::SW:
				rockDirection = Direction::NW;
				break;
			}
		}
	}
}

void Asteroid::die()
{
	isDead = true;
	score += ASTEROID_SCORE;
	if (rockSize == Size::LARGE)
	{
		if (currentLevel != Level::BATTLE_RUINS)
		{
			for (int i = (int)Direction::W; i < (int)Direction::TOTAL; i++)
			{
				tempAsteroids.push_back(Asteroid(pPosX, pPosY, ASTEROID_WIDTH / 2, ASTEROID_HEIGHT / 2, ASTEROID_VEL, Size::MEDIUM, (Direction)i));
			}
		}	
		else
		{
			for (int i = (int)Direction::W; i < (int)Direction::N; i++)
			{
				tempAsteroids.push_back(Asteroid(pPosX, pPosY, ASTEROID_WIDTH / 2, ASTEROID_HEIGHT / 2, ASTEROID_VEL, Size::MEDIUM, (Direction)i));
			}
		}
	}
	else if (rockSize == Size::MEDIUM)
	{
		if (currentLevel != Level::BATTLE_RUINS)
		{
			for (int i = (int)Direction::W; i < (int)Direction::TOTAL; i++)
			{
				tempAsteroids.push_back(Asteroid(pPosX, pPosY, ASTEROID_WIDTH / 4, ASTEROID_HEIGHT / 4, ASTEROID_VEL, Size::SMALL, (Direction)i));
			}
		}
		else
		{
			for (int i = (int)Direction::W; i < (int)Direction::N; i++)
			{
				tempAsteroids.push_back(Asteroid(pPosX, pPosY, ASTEROID_WIDTH / 4, ASTEROID_HEIGHT / 4, ASTEROID_VEL, Size::SMALL, (Direction)i));
			}
		}
	}
}

void Asteroid::render(SDL_Renderer* renderer)
{
	if (!isOnScreen || pPosX < 0) return;
	if(currentLevel == Level::BATTLE_RUINS) angle = angle + 10;
	else if (currentLevel == Level::CITY_LIGHTS) angle = angle + 5;
	else angle = -10;
	if (angle == 360) angle = 0;
	TextureStorage::getInstance().asteroidTexture.setWidth(pWidth);
	TextureStorage::getInstance().asteroidTexture.setHeight(pHeight);
	if (!isDead) TextureStorage::getInstance().asteroidTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer, NULL, angle);
	else if (frame != EXPLOSION_FRAMES)
	{
		SDL_Rect* currentClip = &explosionClips[frame];
		TextureStorage::getInstance().explosionTexture.render(pPosX, pPosY, renderer, currentClip);
		frame++;
	}
}

void Asteroid::shootBullet()
{
	SoundPlayer::getInstance().playShoot(true);
	bullets.push_back(Bullet(pPosX - pHitbox.radius, pPosY, ENEMY_BULLET_WIDTH,
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, false, true));
}

void Asteroid::setHitbox()
{
	if (isDead) pHitbox.x = -SCREEN_WIDTH;
	else
	{
		pHitbox.x = pPosX;
		pHitbox.y = pPosY;
	}
}