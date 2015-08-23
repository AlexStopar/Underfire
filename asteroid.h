#ifndef ASTEROID_H
#define ASTEROID_H
#include <vector>
#include <SDL.h>
#include "movingobj.h"
#include "bullet.h"
#include "common.h"
#include "texturestorage.h"

enum class Size {SMALL, MEDIUM, LARGE};
enum class Direction {W, NW, SW, N, S, TOTAL};
class Asteroid : public IMovingObject
{
public:
	Asteroid(int x, int y, int w, int h, int v, Size s, Direction d) : IMovingObject(x, y, w, h, v) 
	{
		setHitbox();
		rockSize = s;
		rockDirection = d;
		initDirection = rockDirection;
		for (int i = 0; i < EXPLOSION_FRAMES - 1; i++)
		{
			explosionClips[i] = { (TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES * i),
				0, TextureStorage::getInstance().explosionTexture.getWidth() / EXPLOSION_FRAMES, TextureStorage::getInstance().explosionTexture.getHeight() };
		}
	}
	bool isDead = false;
	void move() {}
	void die();
	Size rockSize;
	void move(std::vector<Bullet>& bullets);
	void render(SDL_Renderer* renderer);
private:
	int hp = 3;
	int angle;
	SDL_Rect explosionClips[EXPLOSION_FRAMES];
	Direction rockDirection;
	Direction initDirection;
	void shootBullet();
	void setHitbox();
};

#endif