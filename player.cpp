#include <SDL.h>
#include <vector>
#include "player.h"
#include "common.h"
#include "level.h"
#include "enemy.h"
#include "gate.h"
#include "laser.h"
#include "boss.h"
#include "textdraw.h"
#include "powerup.h"
#include "texturestorage.h"
#include "soundplayer.h"

void Player::handleEvent(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: pVelY -= pVelocity; break;
		case SDLK_DOWN: pVelY += pVelocity; break;
		case SDLK_LEFT: pVelX -= pVelocity; break;
		case SDLK_RIGHT: pVelX += pVelocity; break;
		case SDLK_z: isKeyUp = false;  bulletDelay = 0; break;
		}
	}
	else if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_UP: pVelY += pVelocity; break;
		case SDLK_DOWN: pVelY -= pVelocity; break;
		case SDLK_LEFT: pVelX += pVelocity; break;
		case SDLK_RIGHT: pVelX -= pVelocity; break;
		case SDLK_z: isKeyUp = true; break;
		}
	}
}

void Player::move(std::vector<PowerUp>& powerups, std::vector<Enemy>& enemies, std::vector<Bullet>& bullets, 
	std::vector<Asteroid>& asteroids, std::vector<Gate>& gates, std::vector<Laser>& lasers, Boss& boss)
{
	if (isDead) return;
	if (!isKeyUp && bulletDelay == 0) shootBullet();
	bulletDelay++;
	if (isRapid)
	{
		fullBulletDelay = 1;
		rapidLimit++;
		if (rapidLimit == RAPID_COUNT) isRapid = false;
	}
	else fullBulletDelay = 10;
	if (bulletDelay == fullBulletDelay) bulletDelay = 0;

	if (pVelX != 0 || pVelY != 0) SoundPlayer::getInstance().playShipMove(false);
	bool isColliding = false;
	pPosX += pVelX;
	setHitbox();
	if ((pPosX < 0) || (pPosX + pWidth > SCREEN_WIDTH) || isColliding)
	{
		pPosX -= pVelX;
		setHitbox();
	}

	isColliding = false;
	pPosY += pVelY;
	setHitbox();

	if ((pPosY < 0) || (pPosY + pHeight > SCREEN_HEIGHT) || isColliding)
	{
		pPosY -= pVelY;
		setHitbox();
	}
	for (std::vector<PowerUp>::iterator it = powerups.begin(); it != powerups.end(); ++it) {
		if (checkCollision(pHitbox, it->getHitbox()))
		{
			it->setX(-SCREEN_WIDTH);
			resolvePowerUp(it->powerType);
		}
	}
	for (std::vector<Gate>::iterator it = gates.begin(); it != gates.end(); ++it) {
		if (checkCollision(pHitbox, it->gateBox)) it->TriggerGate();
	}
	if (invincibilityCount == FULL_INVINCIBILITY_COUNT)
	{
		bool isHit = false;
		for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
			if (checkCollision(pHitbox, it->getHitbox()) && it->isOnScreen) isHit = true;
		}
		for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
			if (checkCollision(pHitbox, it->getHitbox()) && it->isEnemy) isHit = true;
		}
		for (std::vector<Laser>::iterator it = lasers.begin(); it != lasers.end(); ++it) {
			if (checkCollision(pHitbox, it->laserBox) && !it->isPreparing && 
				it->isOnScreen && it->getHeight() > SAFE_LASER_HEIGHT) isHit = true;
		}
		for (std::vector<Asteroid>::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
			if (checkCollision(pHitbox, it->getHitbox()) && it->isOnScreen) isHit = true;
		}
		if (checkCollision(pHitbox, boss.getHitbox()) && isBoss) isHit = true;
		if (isHit)
		{
			SoundPlayer::getInstance().playHit(false);
			bulletLevel = 1;
			bulletPower = bulletLevel;
			isRapid = false;
			playerLives.changeLifeCount(1);
			invincibilityCount = 0;
		}
	}
	else invincibilityCount++;
	if (playerLives.getLives() == 0) die();
}

void Player::render(SDL_Renderer* renderer)
{
	if (isDead)
	{
		if (frame != EXPLOSION_FRAMES)
		 {
			SDL_Rect* currentClip = &explosionClips[frame];
			TextureStorage::getInstance().explosionTexture.render(pPosX, pPosY, renderer, currentClip);
			frame++;
		}
	}
	TextureStorage::getInstance().playerTexture.setHeight(PLAYER_HEIGHT);
	TextureStorage::getInstance().playerTexture.setWidth(PLAYER_WIDTH);
	if (invincibilityCount == FULL_INVINCIBILITY_COUNT || invincibilityCount % 2 != 0)
		TextureStorage::getInstance().playerTexture.render(pPosX - pHitbox.radius,
		pPosY - pHitbox.radius, renderer);
	playerLives.renderLives(renderer);
}

void Player::resolvePowerUp(PowerUpType pt)
{
	SoundPlayer::getInstance().playPowerUp(pt);
	switch (pt)
	{
	case PowerUpType::BULLET:
		score += BULLET_SCORE;
		if (bulletLevel != 3) bulletLevel++;
		bulletPower = bulletLevel;
		break;
	case PowerUpType::LIFE:
		playerLives.changeLifeCount(-1);
		break;
	case PowerUpType::RAPID:
		isRapid = true;
		rapidLimit = 0;
		score += RAPID_SCORE;
		break;
	case PowerUpType::SCORE:
		score += SCORE_SCORE;
		break;
	}
}

void Player::setHitbox()
{
	pHitbox.x = pPosX + HORIZONTAL_HIT_OFFSET;
	pHitbox.y = pPosY + VERTICAL_HIT_OFFSET;
}

void Player::shootBullet()
{
	SoundPlayer::getInstance().playShoot(false);
	int texWidth = TextureStorage::getInstance().playerTexture.getWidth();
	if (bulletLevel == 1 || bulletLevel == 3) bullets.push_back(Bullet(pPosX + texWidth, pPosY + VERTICAL_HIT_OFFSET, 
		PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT, PLAYER_BULLET_VEL,MoveType::FORWARD, false, false));
	if (bulletLevel == 2 || bulletLevel == 3)
	{
		bullets.push_back(Bullet(pPosX + texWidth, pPosY + VERTICAL_HIT_OFFSET,
			PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT, PLAYER_BULLET_VEL, MoveType::UP, false, false));
		bullets.push_back(Bullet(pPosX + texWidth, pPosY + VERTICAL_HIT_OFFSET,
			PLAYER_BULLET_WIDTH, PLAYER_BULLET_HEIGHT, PLAYER_BULLET_VEL, MoveType::DOWN, false, false));
	}
}

void Player::die()
{
	SoundPlayer::getInstance().playDeath(false);
	SoundPlayer::getInstance().stopMusic();
	SoundPlayer::getInstance().playMusic(true, false);
	isDead = true;
}