#ifndef MOVINGOBJ_H
#define MOVINGOBJ_H
#include <SDL.h>
#include <vector>
#include "texture.h"
#include "collision.h"

class IMovingObject
{
public:
	IMovingObject(int x, int y, int w, int h, int v)
	{
		pPosX = x;
		pPosY = y;
		pWidth = w;
		pHeight = h;
		pVelocity = v;
		pHitbox.radius = w / 2;
		pVelX = 0;
		pVelY = 0;
	}
	bool isOnScreen = true;
	virtual void move() = 0;
	virtual void render(SDL_Renderer* renderer) = 0;
	Circle& getHitbox() { return pHitbox; }
	virtual void setHitbox() = 0;
	int getX() { return pPosX; }
	int getY() { return pPosY; }
	void setX(int x) { pPosX = x; }
	void setY(int y) { pPosY = y; }
protected:
	int pWidth, pHeight;
	int pVelocity;
	int pPosX, pPosY;
	int pVelX, pVelY;
	int frame = 0;
	Circle pHitbox;
};
#endif 
