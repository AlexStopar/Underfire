#ifndef POWERUP_H
#define POWERUP_H
#include "movingobj.h"
#include "texturestorage.h"

enum class PowerUpType {BULLET, LIFE, SCORE, RAPID};
const int POWERUP_FRAMES = 4;
class PowerUp : public IMovingObject
{
public:
	PowerUp(int x, int y, int w, int h, int v, PowerUpType pt) : IMovingObject(x, y, w, h, v) {
		powerType = pt;
		powerUpClips[0] = { 0, 0, pWidth / 2, pHeight / 2 };
		powerUpClips[1] = { pWidth / 2, 0, pWidth / 2, pHeight / 2 };
		powerUpClips[2] = { 0, pHeight / 2, pWidth / 2, pHeight / 2 };
		powerUpClips[3] = { pWidth / 2, pHeight / 2, pWidth / 2, pHeight / 2 };
	}
	void move();
	PowerUpType powerType;
	void render(SDL_Renderer* renderer);
private:
	void setHitbox();
	int waveCount = 0;
	const int MAX_WAVE_COUNT = 100;
	SDL_Rect powerUpClips[POWERUP_FRAMES];	
};
#endif