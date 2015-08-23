#ifndef COMMON_H
#define COMMON_H
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <string>


const int BOSS_HIT_OFFSET_X = 200;
const int BOSS_HIT_OFFSET_Y = 350;
const int BOSS_HIT_OFFSET2_X = 350;
const int BOSS_HIT_OFFSET3_X = 20;
const int BOSS_HIT_OFFSET2_Y = 170;
const int BOSS_HIT_OFFSET3_Y = 300;
const int LEVEL_3_ENEMY_CLIPS = 2;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_WIDTH = 80;
const int PLAYER_HEIGHT = 60;
const int OFFSCREEN_MOVE_LIMIT = 2;
const int BOSS_WIDTH = 100;
const int BOSS_HEIGHT = 100;
const int ASTEROID_WIDTH = 300;
const int ASTEROID_HEIGHT = 264;
const int PLAYER_BULLET_WIDTH = 14;
const int PLAYER_BULLET_HEIGHT = 10;
const int GATE_WIDTH = 73;
const int LASER_HEIGHT = 30;
const int GATE_HEIGHT = 600;
const int GATE_VEL = 1;
const int ENEMY_BULLET_WIDTH = 34;
const int ENEMY_BULLET_HEIGHT = 32;
const int ENEMY_EASY_WIDTH = 56;
const int ENEMY_EASY2_WIDTH = 124;
const int ENEMY_EASY2_ANGLE = 45;
const int ENEMY_EASY_HEIGHT = 80;
const int ENEMY_MEDIUM_WIDTH = 80;
const int ENEMY_MEDIUM_HEIGHT = 94;
const int ENEMY_MEDIUM3_HEIGHT = 64;
const int ENEMY_MEDIUM3_WIDTH = 102;
const int LIFE_WIDTH = 40;
const int SAFE_LASER_HEIGHT = 10;
const int LIFE_HEIGHT = 30;
const int POWERUP_WIDTH = 100;
const int POWERUP_HEIGHT = 100;
const int PLAYER_HIT_WIDTH = 12;
const int PLAYER_HIT_HEIGHT = 10;
const int PLAYER_VEL = 5;
const int POWERUP_VEL = 1;
const int BOSS_VEL = 1;
const int ASTEROID_VEL = 2;
const int ENEMY_VEL = 6;
const int SCROLL_SPEED = 2;
const int SPEED_MULTIPLIER = 2;
const int ENEMY_MED_VEL = 1;
const int PLAYER_BULLET_VEL = 10;
const int ENEMY_BULLET_VEL = 6;
const int ENEMY_BULLET2_VEL = 8;
const int BOSS2_BULLET_VEL = 5;
const int BULLET_SCORE = 1000;
const int ASTEROID_SCORE = 300;
const int ENEMY_EASY_SCORE = 200;
const int ENEMY_MEDIUM_SCORE = 600;
const int RAPID_SCORE = 500;
const int SCORE_SCORE = 2000;
const int BOSS_SCORE = 20000;
const int DISPLAY_LIMIT = 60;
const int EXPLOSION_FRAMES = 25;

extern int score;
extern int setLives;
extern int bulletPower;
extern int firstBulletDelay;
extern int frameCount;
extern bool isResetting;
extern bool isQuitting;
extern bool isFirstBulletShot;
extern bool isContinuing;
extern bool isWon;
extern bool isPaused;
extern bool isWindowed;
extern bool isSpeedy;

enum class Level {BATTLE_RUINS, CITY_LIGHTS, SPEED_BRIDGE, TOTAL};
extern Level currentLevel;
void toggleFullscreen(SDL_Window* window);
int resolveEnemyEasyWidth();
int resolveEnemyEasyHeight();
int resolveEnemyMediumWidth();
int resolveEnemyBulletVel();
int resolveMediumVel();
int resolveBossOffset(bool isY);
bool resolveGrowingBullets(bool isEasy);
std::string resolveLevelFile();

#endif