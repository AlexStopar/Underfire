#ifndef BOSS_H
#define BOSS_H
#include "movingobj.h"
#include "texturestorage.h"
#include "bullet.h"
#include "common.h"

enum ShootType {STRAIGHTFIRE, CRAZYFIRE, CRAZIERFIRE};
extern bool isBoss;
class Boss : public IMovingObject
{
public:
	Boss(int x, int y, int w, int h, int v) : IMovingObject(x, y, w, h, v)
	{
		assignHP();
		setHitbox();
		for (int i = 0; i < EXPLOSION_FRAMES - 1; i++)
		{
			explosionClips[i] = { (TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES * i),
				0, TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES, TextureStorage::getInstance().explosionTexture.getHeight() };
		}
		laserLimit = LASER_LIMIT;
		if (currentLevel == Level::SPEED_BRIDGE) bulletLimit = BOSS3_BULLET_LIMIT;
		else bulletLimit = BOSS_BULLET_LIMIT;
	}
	bool isDead = false;
	void move() {}
	void render(SDL_Renderer* renderer);
	void move(std::vector<Bullet>& bullets, bool isPlayerDead);
private:
	const int LASER_LIMIT = 40;
	int bulletLimit;
	const int BOSS_HP = 80;
	const int BOSS2_HP = 120;
	const int BOSS3_HP = 150;
	SDL_Rect explosionClips[EXPLOSION_FRAMES];
	int angle = 0;
	int hp;
	int laserLimit;
	ShootType sType = ShootType::STRAIGHTFIRE;
	const int BOSS_BULLET_LIMIT = 3;
	const int BOSS3_BULLET_LIMIT = 5;
	int frame = EXPLOSION_FRAMES + 1;
	int explosionX;
	int explosionY;
	void shootBullet();
	void shootLaser();
	void die();
	int getHP();
	void assignHP();
	void setHitbox();
};


#endif