#ifndef LEVEL_H
#define LEVEL_H
#include <SDL.h>
#include "texture.h" 
#include "bullet.h"
#include "asteroid.h"
#include "laser.h"


extern std::vector<Bullet> bullets;
extern std::vector<Laser> lasers;
extern std::vector<Asteroid> tempAsteroids;
extern std::vector<Bullet> tempBullets;
bool loadMedia(SDL_Renderer* renderer, bool isReloading);
void playLevel(SDL_Renderer* renderer, SDL_Window* window);
void reset();

#endif 
