#include "texturestorage.h"
#include "texture.h"
#include "common.h"
#include <SDL.h>

bool TextureStorage::loadTextures(SDL_Renderer* renderer)
{
	playerTexture.loadFromFile("playerShip.png", renderer);
	bulletTexture.loadFromFile("playerBullet.png", renderer);
	powerUpTexture.loadFromFile("powerup.png", renderer);
	explosionTexture.loadFromFile("explosion.png", renderer);
	gateTexture.loadFromFile("gate.png", renderer);
	laserTexture.loadFromFile("laser.png", renderer);
	switch (currentLevel)
	{
	case Level::BATTLE_RUINS:
		bgTexture.loadFromFile("ufbg.png", renderer);
		asteroidTexture.loadFromFile("asteroid.png", renderer);
		enemyBulletTexture.loadFromFile("enemyBullet.png", renderer);
		enemyEasyTexture.loadFromFile("enemyEasy.png", renderer);
		enemyMediumTexture.loadFromFile("enemyMedium.png", renderer);
		bossTexture.loadFromFile("boss.png", renderer);
		break;
	case Level::CITY_LIGHTS:
		bgTexture.loadFromFile("ufbg2.png", renderer);
		asteroidTexture.loadFromFile("asteroid2.png", renderer);
		enemyEasyTexture.loadFromFile("enemyEasy2.png", renderer);
		enemyBulletTexture.loadFromFile("enemyBullet2.png", renderer);
		enemyMediumTexture.loadFromFile("enemyMedium2.png", renderer);
		bossTexture.loadFromFile("boss2.png", renderer);
		break;
	case Level::SPEED_BRIDGE:
		bgTexture.loadFromFile("ufbg3.png", renderer);
		asteroidTexture.loadFromFile("asteroid3.png", renderer);
		enemyEasyTexture.loadFromFile("enemyEasy3.png", renderer);
		enemyBulletTexture.loadFromFile("enemyBullet3.png", renderer);
		enemyMediumTexture.loadFromFile("enemyMedium3.png", renderer);
		bossTexture.loadFromFile("boss3.png", renderer);
		break;
	}
	int bgSizeMultiplier = SCREEN_HEIGHT / bgTexture.getHeight();
	bgTexture.setHeight(SCREEN_HEIGHT);
	bgTexture.setWidth(bgTexture.getWidth() * bgSizeMultiplier);
	if (bgTexture.getWidth() < SCREEN_WIDTH) bgTexture.setWidth(SCREEN_WIDTH);
	return true;
}

void TextureStorage::free()
{
	bgTexture.free();
	playerTexture.free();
	bulletTexture.free();
	powerUpTexture.free();
	enemyEasyTexture.free();
	explosionTexture.free();
	enemyBulletTexture.free();
	laserTexture.free();
}


