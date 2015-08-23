#ifndef ENEMY_H
#define ENEMY_H
#include <SDL.h>
#include <vector>
#include "common.h"
#include "bullet.h"
#include "texturestorage.h"
#include "movingobj.h"

enum class EnemyType {EASY, MEDIUM};
enum class EnemyMoveType {STRAIGHT, CURVEDOWN, CURVEUP};
class Enemy : public IMovingObject
{
public:
	Enemy(int x, int y, int w, int h, int v, EnemyType et, EnemyMoveType emt) : IMovingObject(x, y, w, h, v) {
		setHitbox();
		eType = et;
		mType = emt;
		for (int i = 0; i < EXPLOSION_FRAMES - 1; i++)
		{
			explosionClips[i] = { (TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES * i), 
				0, TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES, TextureStorage::getInstance().explosionTexture.getHeight() };
		}
		if (currentLevel == Level::CITY_LIGHTS || currentLevel == Level::SPEED_BRIDGE)
		{
			if (emt == EnemyMoveType::CURVEDOWN)
			{
				angle = -ENEMY_EASY2_ANGLE;
				setAngle = LEVEL_THREE_ANGLE;
			}
			else if (emt == EnemyMoveType::CURVEUP)
			{
				angle = ENEMY_EASY2_ANGLE;
				setAngle = -LEVEL_THREE_ANGLE;
			}
		}
		if (currentLevel == Level::SPEED_BRIDGE)
		{
			for (int i = 0; i < LEVEL_3_ENEMY_CLIPS; i++)
			{
				levelThreeEnemyClips[i] = { (ENEMY_MEDIUM3_WIDTH / LEVEL_3_ENEMY_CLIPS * i),
					0, ENEMY_MEDIUM3_WIDTH / LEVEL_3_ENEMY_CLIPS, ENEMY_MEDIUM3_HEIGHT };
			}
		}
	}
	bool isDead = false;
	void move() {}
	void render(SDL_Renderer* renderer);
	void move(std::vector<Bullet>& bullets, bool isAbovePlayer);
private:
	SDL_Rect explosionClips[EXPLOSION_FRAMES];
	SDL_Rect levelThreeEnemyClips[LEVEL_3_ENEMY_CLIPS];
	int hp;
	int angle = 0;
	int setAngle = 0;
	const int LEVEL_THREE_ANGLE = 2;
	EnemyType eType;
	bool isShooting = true;
	EnemyMoveType mType;
	void setHitbox();
	void shootBullet(bool isAbovePlayer);
	void assignHP();
};

#endif 
