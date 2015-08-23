#include <SDL.h>
#include <vector>
#include <random>
#include "boss.h"
#include "level.h"
#include "textdraw.h"
#include "bullet.h"
#include "common.h"
#include "laser.h"
#include "soundplayer.h"

bool isBoss = false;
int firstBulletDelay = 0;
bool isFirstBulletShot = false;
const int FIRST_BULLET_DELAY_MAX = 100;

void Boss::move(std::vector<Bullet>& bullets, bool isPlayerDead)
{
	if (pPosX > -(TextureStorage::getInstance().bossTexture.getWidth() + ENEMY_EASY_WIDTH) && pPosX < SCREEN_WIDTH)
	{
		if (!isBoss)
		{
			SoundPlayer::getInstance().stopMusic();
			SoundPlayer::getInstance().playMusic(false, true);
			isBoss = true;
		}
		isOnScreen = true;
	}
	else isOnScreen = false;

	if (!isOnScreen)
	{
		if (frameCount % OFFSCREEN_MOVE_LIMIT == 0) pPosX--;
		return;
	}
	if (isDead)
	{
		pVelX = -2*pVelocity;
		pVelY = 2*pVelocity;
	}
	else if ((pPosX > (2*SCREEN_WIDTH)/3 && currentLevel == Level::BATTLE_RUINS) || 
		(pPosX > SCREEN_WIDTH/2 && currentLevel == Level::CITY_LIGHTS) || 
		(currentLevel == Level::SPEED_BRIDGE && pPosX > (7*SCREEN_WIDTH)/8) || isPlayerDead) pVelX = -pVelocity;
	else 
	{ 
		if (!isFirstBulletShot && currentLevel == Level::CITY_LIGHTS)
		{
			shootBullet();
			isFirstBulletShot = true;
		}
		pVelX = 0; 
	}
	if (isFirstBulletShot && firstBulletDelay < FIRST_BULLET_DELAY_MAX) firstBulletDelay++;
	pPosX += pVelX;
	pPosY += pVelY;
	setHitbox();
	if (!isDead && frameCount % bulletLimit == 0 && !isPlayerDead)
	{
		if(isFirstBulletShot && firstBulletDelay == FIRST_BULLET_DELAY_MAX && currentLevel == Level::CITY_LIGHTS) shootBullet();
		else if(currentLevel != Level::CITY_LIGHTS) shootBullet();
	}
	if (!isDead && !isPlayerDead && frameCount % laserLimit == 0 && currentLevel == Level::SPEED_BRIDGE) shootLaser();
	for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
		if (checkCollision(pHitbox, it->getHitbox()) && !(it->isEnemy))
		{
			hp--;
			it->setX(2 * SCREEN_WIDTH);
			it->setHitbox();
			if (hp <= 0 && !isDead) die();
			else if(!isDead) SoundPlayer::getInstance().playHit(true);
			if (hp == getHP() / 2) laserLimit = laserLimit / 2;
			if (hp <= getHP() / 2) sType = ShootType::CRAZYFIRE;
			if (hp == getHP() / 4)
			{
				SoundPlayer::getInstance().playCoupleMoreShots();
				laserLimit = laserLimit / 2;
			}
			if (hp <= getHP() / 4 && currentLevel != Level::BATTLE_RUINS) sType = ShootType::CRAZIERFIRE;
		}
	}
}

void Boss::die()
{
	isDead = true;
	score += BOSS_SCORE;
	explosionX = pHitbox.x;
	explosionY = pHitbox.y;
}

void Boss::shootLaser()
{
	SoundPlayer::getInstance().playShoot(true);
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> randomY(pPosY, pPosY + TextureStorage::getInstance().bossTexture.getHeight());
	lasers.push_back(Laser(0, randomY(eng), pPosX + TextureStorage::getInstance().bossTexture.getWidth()/2, 1, 0));
}

void Boss::shootBullet()
{
	if (sType == ShootType::STRAIGHTFIRE && currentLevel == Level::SPEED_BRIDGE) return;
	SoundPlayer::getInstance().playShoot(true);
	if (currentLevel == Level::CITY_LIGHTS && !isFirstBulletShot)
	{
		bullets.push_back(Bullet(pPosX + resolveBossOffset(false), pPosY + resolveBossOffset(true) - ENEMY_BULLET_HEIGHT, ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, false));
		return;
	}
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_int_distribution<> randomY(pPosY, pPosY + TextureStorage::getInstance().bossTexture.getHeight());
	std::uniform_int_distribution<> randomBelow(0, SCREEN_WIDTH);
	std::uniform_int_distribution<> randomSet(0, 1);
	switch (sType)
	{
	case ShootType::STRAIGHTFIRE:
		if(currentLevel == Level::BATTLE_RUINS) bullets.push_back(Bullet(pPosX + resolveBossOffset(false), randomY(eng), ENEMY_BULLET_WIDTH, 
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, false));
		else bullets.push_back(Bullet(randomBelow(eng), SCREEN_HEIGHT, ENEMY_BULLET_WIDTH,
			ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::STRAIGHTUP, true, false));
		break;
	case ShootType::CRAZYFIRE:
		if (currentLevel == Level::BATTLE_RUINS)
		{
			bullets.push_back(Bullet(pPosX + resolveBossOffset(false), randomY(eng), ENEMY_BULLET_WIDTH, 
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DIAGONAL, true, !!randomSet(eng)));
		}
		else if(currentLevel == Level::CITY_LIGHTS)
		{
			if (!!randomSet(eng))
			{
				bullets.push_back(Bullet(randomBelow(eng), SCREEN_HEIGHT, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::STRAIGHTUP, true, false));
			}
			else
			{
				bullets.push_back(Bullet(randomBelow(eng), 0, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::STRAIGHTDOWN, true, false));
			}
			
		}
		else bullets.push_back(Bullet(pPosX + resolveBossOffset(false), randomY(eng), ENEMY_BULLET_WIDTH, 
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::FORWARD, true, false));
		break;
	case ShootType::CRAZIERFIRE:
		if (currentLevel == Level::CITY_LIGHTS)
		{
			if (!!randomSet(eng))
			{
				bullets.push_back(Bullet(randomBelow(eng), SCREEN_HEIGHT, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::NEGDIAGONAL, true, false));
				bullets.push_back(Bullet(randomBelow(eng), SCREEN_HEIGHT, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::DIAGONAL, true, false));
			}
			else
			{
				bullets.push_back(Bullet(randomBelow(eng), 0, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::DIAGONAL, true, true));
				bullets.push_back(Bullet(randomBelow(eng), 0, ENEMY_BULLET_WIDTH,
					ENEMY_BULLET_HEIGHT, BOSS2_BULLET_VEL, MoveType::NEGDIAGONAL, true, true));
			}
			bullets.push_back(Bullet(pPosX + resolveBossOffset(false), randomY(eng), ENEMY_BULLET_WIDTH,
				ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DIAGONAL, true, !!randomSet(eng)));
		}
		else bullets.push_back(Bullet(pPosX + resolveBossOffset(false), randomY(eng), ENEMY_BULLET_WIDTH,
			ENEMY_BULLET_HEIGHT, resolveEnemyBulletVel(), MoveType::DIAGONAL, true, !!randomSet(eng)));
		break;
	}
}

void Boss::assignHP()
{
	if(currentLevel == Level::BATTLE_RUINS) hp = BOSS_HP;
	else if(currentLevel == Level::CITY_LIGHTS) hp = BOSS2_HP;
	else hp = BOSS3_HP;
}

int Boss::getHP()
{
	if (currentLevel == Level::BATTLE_RUINS) return BOSS_HP;
	else if(currentLevel == Level::CITY_LIGHTS) return BOSS2_HP;
	return BOSS3_HP;
}

void Boss::setHitbox()
{
	if (isDead) pHitbox.x = -SCREEN_WIDTH;
	else
	{
		pHitbox.x = pPosX + resolveBossOffset(false);
		pHitbox.y = pPosY + resolveBossOffset(true);
		if (currentLevel == Level::SPEED_BRIDGE) pHitbox.radius = pWidth / 4;
	}
}

void Boss::render(SDL_Renderer* renderer)
{
	if (!isOnScreen) return;
	TextureStorage::getInstance().bossTexture.render(pPosX - pHitbox.radius, pPosY - pHitbox.radius, renderer, NULL, angle);
	if (isDead && !isPaused)
	{
		angle++;
		if (frame < EXPLOSION_FRAMES)
		{
			SDL_Rect* currentClip = &explosionClips[frame];
			TextureStorage::getInstance().explosionTexture.render(explosionX, explosionY, renderer, currentClip);
			frame++;
		}
		else
		{
			frame = 0;
			SoundPlayer::getInstance().playDeath(true);
			std::random_device rd;
			std::mt19937 eng(rd());
			std::uniform_int_distribution<> randomX(pPosX + ENEMY_EASY_WIDTH, pPosX + TextureStorage::getInstance().bossTexture.getWidth() - ENEMY_EASY_WIDTH);
			std::uniform_int_distribution<> randomY(pPosY + ENEMY_EASY_HEIGHT, pPosY + TextureStorage::getInstance().bossTexture.getHeight() - ENEMY_EASY_HEIGHT);
			explosionX = randomX(eng);
			explosionY = randomY(eng);
		}
	}
	else if (!isDead) TextDraw::getInstance().renderBossMeter(renderer, hp);
}