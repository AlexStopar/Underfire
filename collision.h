#ifndef COLLISION_H
#define COLLISION_H
#include <SDL.h>

struct Circle
{
	int x, y;
	int radius;
};
double distanceSquared(int x1, int y1, int x2, int y2);
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Circle& a, SDL_Rect b);
#endif