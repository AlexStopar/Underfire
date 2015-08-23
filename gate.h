#ifndef GATE_H
#define GATE_H
#include "movingobj.h"
#include <SDL.h>

class Gate : public IMovingObject
{
public:
	Gate(int x, int y, int w, int h, int v) : IMovingObject(x, y, w, h, v) {
		gateBox = { x, y, w, h };
	}
	void TriggerGate();
	void move();
	SDL_Rect gateBox;
	void render(SDL_Renderer* renderer);
private:
	void setHitbox();
	bool isTriggered = false;
};


#endif