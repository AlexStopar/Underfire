#ifndef BULLET_H
#define BULLET_H 
#include "movingobj.h"
#include <SDL.h>
#include <cmath>

enum class MoveType {FORWARD, UP, DOWN, DIAGONAL, NEGDIAGONAL, STRAIGHTUP, STRAIGHTDOWN};

class Bullet : public IMovingObject
{
public:
	Bullet(int x, int y, int w, int h, int v, MoveType mt, bool isBad, bool isAbove, bool isGrowable = false, bool isShootable = false) : IMovingObject(x, y, w, h, v) {
		bulletMovementType = mt;
		isEnemy = isBad;
		isAbovePlayer = isAbove;
		isGrowing = isGrowable;
		isShooting = isShootable;
	}
	void move();
	void render(SDL_Renderer* renderer);
	bool isEnemy;
	void setHitbox();
private:
	MoveType bulletMovementType;
	bool isAbovePlayer;
	int angle = 0;
	bool isShooting;
	bool isGrowing;
	int leanTime = 0;	
	void shootBullet();
};
#endif
