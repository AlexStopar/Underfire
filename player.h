#ifndef PLAYER_H
#define PLAYER_H
#include <SDL.h>
#include <vector>
#include "movingobj.h"
#include "lives.h"
#include "asteroid.h"
#include "common.h"
#include "boss.h"
#include "enemy.h"
#include "laser.h"
#include "gate.h"
#include "powerup.h"
#include "texturestorage.h"

class Player : public IMovingObject
{
public:
	bool isDead = false;
	Player(int x, int y, int w, int h, int v) : IMovingObject(x, y, w, h, v) {
		playerLives = Lives();
		playerLives.setLives(setLives);
		bulletLevel = bulletPower;
		setHitbox();
		for (int i = 0; i < EXPLOSION_FRAMES - 1; i++)
		{
			explosionClips[i] = { (TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES * i),
				0, TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES, TextureStorage::getInstance().explosionTexture.getHeight() };
		}
	}
	Lives playerLives;
	void handleEvent(SDL_Event& e);
	void move() {}
	void move(std::vector<PowerUp>& powerups, std::vector<Enemy>& enemies, 
		std::vector<Bullet>& bullets, std::vector<Asteroid>& asteroids, std::vector<Gate>& gates, std::vector<Laser>& lasers, Boss& boss);
	void resolvePowerUp(PowerUpType pt);
	void shootBullet();
	void die();
	void render(SDL_Renderer* renderer);
private:
	SDL_Rect explosionClips[EXPLOSION_FRAMES];
	const int FULL_INVINCIBILITY_COUNT = 50;
	const int RAPID_COUNT = 150;
	int fullBulletDelay = 10;
	int rapidLimit = 0;
	bool isRapid = false;
	const int HORIZONTAL_HIT_OFFSET = 48;
	const int VERTICAL_HIT_OFFSET = 24;
	int bulletLevel = 1;
	bool isKeyUp = true;
	int bulletDelay = 0;
	int invincibilityCount = FULL_INVINCIBILITY_COUNT;
	void setHitbox();
};


#endif