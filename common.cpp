#include <SDL.h>
#include <string>
#include "common.h"

bool isWindowed = true;
bool isResetting = true;
bool isQuitting = false;
int score = 0;

int resolveEnemyEasyWidth()
{
	if (currentLevel == Level::CITY_LIGHTS) return ENEMY_EASY2_WIDTH;
	return ENEMY_EASY_WIDTH;
}

int resolveEnemyEasyHeight()
{
	if (currentLevel == Level::SPEED_BRIDGE) return ENEMY_EASY_WIDTH;
	return ENEMY_EASY_HEIGHT;
}

int resolveEnemyMediumWidth()
{
	if (currentLevel == Level::BATTLE_RUINS) return ENEMY_MEDIUM_WIDTH;
	else if(currentLevel == Level::CITY_LIGHTS) return ENEMY_EASY2_WIDTH;
	return ENEMY_MEDIUM3_WIDTH/LEVEL_3_ENEMY_CLIPS;
}

int resolveEnemyBulletVel()
{
	if (currentLevel == Level::BATTLE_RUINS) return ENEMY_BULLET_VEL;
	return ENEMY_BULLET2_VEL;
}

int resolveMediumVel()
{
	if (currentLevel == Level::BATTLE_RUINS) return ENEMY_VEL;
	return ENEMY_MED_VEL;
}

int resolveBossOffset(bool isY)
{
	if (currentLevel == Level::BATTLE_RUINS && isY) return BOSS_HIT_OFFSET_Y;
	else if(currentLevel == Level::CITY_LIGHTS && isY) return BOSS_HIT_OFFSET2_Y;
	else if (currentLevel == Level::SPEED_BRIDGE && isY) return BOSS_HIT_OFFSET3_Y;
	else if (currentLevel == Level::BATTLE_RUINS) return BOSS_HIT_OFFSET_X;
	else if (currentLevel == Level::SPEED_BRIDGE) return BOSS_HIT_OFFSET3_X;
	return BOSS_HIT_OFFSET2_X;
}

bool resolveGrowingBullets(bool isEasy)
{
	if (isEasy && currentLevel != Level::SPEED_BRIDGE) return false;
	else if (currentLevel == Level::BATTLE_RUINS) return false;
	return true;
}

std::string resolveLevelFile()
{
	if (currentLevel == Level::BATTLE_RUINS) return "StageOne.csv";
	else if(currentLevel == Level::CITY_LIGHTS) return "StageTwo.csv";
	return "StageThree.csv";
}


void toggleFullscreen(SDL_Window* window)
{
	if (isWindowed)
	{
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		isWindowed = false;
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		isWindowed = true;
	}
}
