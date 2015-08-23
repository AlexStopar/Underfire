#ifndef LASER_H
#define LASER_H
#include "movingobj.h"

class Laser : public IMovingObject
{
public:
	Laser(int x, int y, int w, int h, int v) : IMovingObject(x, y, w, h, v) {
		laserBox = { x, y, w, h };
		isOnScreen = true;
	}
	void move();
	SDL_Rect laserBox;
	bool isPreparing = true;
	int getHeight() { return pHeight; }
	void render(SDL_Renderer* renderer);
private:
	int timer = 0;
	bool isRising = false;
	bool isFalling = false;
	void setHitbox();
};








#endif