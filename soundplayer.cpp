#include <SDL.h>
#include <SDL_mixer.h>
#include "soundplayer.h"
#include "powerup.h"
#include "common.h"

Mix_Music* stageTheme = NULL;
Mix_Music* bossTheme = NULL;
Mix_Chunk* shipMoveSound = NULL;
Mix_Chunk* playerShootSound = NULL;
Mix_Chunk* laserSound = NULL;
Mix_Chunk* enemyShootSound = NULL;
Mix_Chunk* powerUpSound = NULL;
Mix_Chunk* gainLifeSound = NULL;
Mix_Chunk* highScoreSound = NULL;
Mix_Chunk* gateTriggerSound = NULL;
Mix_Chunk* enemyShipMoveSound = NULL;
Mix_Chunk* enemyDeathSound = NULL;
Mix_Chunk* playerDeathSound = NULL;
Mix_Chunk* playerHitSound = NULL;
Mix_Chunk* enemyHitSound = NULL;
Mix_Music* gameOverTheme = NULL;
Mix_Chunk* levelCompleteSound = NULL;
Mix_Chunk* coupleMoreShots = NULL;
Mix_Chunk* pauseSound = NULL;
Mix_Chunk* menuPickSound = NULL;
Mix_Chunk* menuSwitchSound = NULL;

bool SoundPlayer::loadSounds()
{
	switch (currentLevel)
	{
	case Level::BATTLE_RUINS:
		stageTheme = Mix_LoadMUS("stageTheme.wav");
		break;
	case Level::CITY_LIGHTS:
		stageTheme = Mix_LoadMUS("stage2Theme.wav");
		break;
	case Level::SPEED_BRIDGE:
		stageTheme = Mix_LoadMUS("stage3Theme.wav");
		break;
	}
	
	bossTheme = Mix_LoadMUS("bossTheme.wav");
	gameOverTheme = Mix_LoadMUS("gameOverTheme.wav");
	shipMoveSound = Mix_LoadWAV("shipMoveSound.wav");
	pauseSound = Mix_LoadWAV("pauseSound.wav");
	menuPickSound = Mix_LoadWAV("menuPickSound.wav");
	menuSwitchSound = Mix_LoadWAV("menuSwitchSound.wav");
	playerShootSound = Mix_LoadWAV("playerShootSound.wav");
	enemyShipMoveSound = Mix_LoadWAV("enemyShipMoveSound.wav");
	powerUpSound = Mix_LoadWAV("powerUpSound.wav");
	playerHitSound = Mix_LoadWAV("playerHitSound.wav");
	enemyHitSound = Mix_LoadWAV("enemyHitSound.wav");
	levelCompleteSound = Mix_LoadWAV("levelCompleteSound.wav");
	gainLifeSound = Mix_LoadWAV("gainLifeSound.wav");
	enemyDeathSound = Mix_LoadWAV("enemyDeathSound.wav");
	enemyShootSound = Mix_LoadWAV("enemyShootSound.wav");
	playerDeathSound = Mix_LoadWAV("playerDeathSound.wav");
	coupleMoreShots = Mix_LoadWAV("coupleMoreShots.wav");
	highScoreSound = Mix_LoadWAV("bigMoneyBigPrizes.wav");
	gateTriggerSound = Mix_LoadWAV("gateTriggerSound.wav");
	laserSound = Mix_LoadWAV("laserSound.wav");
	return true;
}

void SoundPlayer::playMusic(bool isDead, bool isBoss)
{
	if (isDead) Mix_PlayMusic(gameOverTheme, -1);
	else if (isBoss) Mix_PlayMusic(bossTheme, -1);
	else  Mix_PlayMusic(stageTheme, -1);
}

void SoundPlayer::stopSound()
{
	Mix_HaltChannel(2);
}

void SoundPlayer::playLaser()
{
	Mix_PlayChannel(1, laserSound, 0);
}

void SoundPlayer::pauseMusic()
{
	if (Mix_PausedMusic() == 1) Mix_ResumeMusic();
	else Mix_PauseMusic();
}

void SoundPlayer::stopMusic()
{
	Mix_HaltMusic();
}

void SoundPlayer::playPause()
{
	Mix_PlayChannel(2, pauseSound, 0);
}

void SoundPlayer::playHighScore()
{
	Mix_PlayChannel(2, highScoreSound, 0);
}

void SoundPlayer::playMenuPick()
{
	Mix_PlayChannel(2, menuPickSound, 0);
}

void SoundPlayer::playGateTrigger()
{
	Mix_PlayChannel(1, gateTriggerSound, 0);
}

void SoundPlayer::playMenuSwitch()
{
	Mix_PlayChannel(2, menuSwitchSound, 0);
}

void SoundPlayer::playPowerUp(PowerUpType pt)
{
	if (pt == PowerUpType::LIFE) Mix_PlayChannel(3, gainLifeSound, 0);
	else Mix_PlayChannel(3, powerUpSound, 0);
}

void SoundPlayer::playShipMove(bool isEnemy)
{
	Mix_VolumeChunk(shipMoveSound, MOVE_VOLUME);
	Mix_VolumeChunk(enemyShipMoveSound, MOVE_VOLUME);
	if(!isEnemy) Mix_PlayChannel(-1, shipMoveSound, 0);
	else Mix_PlayChannel(4, enemyShipMoveSound, 0);
}

void SoundPlayer::playCoupleMoreShots()
{
	Mix_PlayChannel(2, coupleMoreShots, 0);
}

void SoundPlayer::playWin()
{
	Mix_PlayChannel(2, levelCompleteSound, 0);
}

void SoundPlayer::playShoot(bool isEnemy)
{
	if (!isEnemy)
	{
		Mix_VolumeChunk(playerShootSound, SHOOT_VOLUME);
		Mix_PlayChannel(5, playerShootSound, 0);
	}
	else
	{
		Mix_PlayChannel(6, enemyShootSound, 0);
	}
}
	
void SoundPlayer::playHit(bool isEnemy)
{
	if (isEnemy) Mix_PlayChannel(3, enemyHitSound, 0);
	else Mix_PlayChannel(3, playerHitSound, 0);
}

void SoundPlayer::playDeath(bool isEnemy)
{
	if(isEnemy) Mix_PlayChannel(3, enemyDeathSound, 0);
	else Mix_PlayChannel(3, playerDeathSound, 0);
}

void SoundPlayer::free()
{
	Mix_FreeMusic(stageTheme);
	Mix_FreeMusic(gameOverTheme);
	Mix_FreeChunk(shipMoveSound);
	Mix_FreeChunk(playerShootSound);
	Mix_FreeChunk(powerUpSound);
	Mix_FreeChunk(gainLifeSound);
	Mix_FreeChunk(enemyDeathSound);
	Mix_FreeChunk(enemyShipMoveSound);
	Mix_FreeChunk(enemyShootSound);
	Mix_FreeChunk(playerHitSound);
	Mix_FreeChunk(laserSound);
	Mix_FreeChunk(levelCompleteSound);
	Mix_FreeChunk(enemyHitSound);
	Mix_FreeChunk(playerDeathSound);
	Mix_FreeChunk(pauseSound);
	Mix_FreeChunk(coupleMoreShots);
	Mix_FreeChunk(menuPickSound);
	Mix_FreeChunk(menuSwitchSound);
	Mix_FreeChunk(highScoreSound);
	Mix_FreeChunk(gateTriggerSound);
	gateTriggerSound = NULL;
	coupleMoreShots = NULL;
	pauseSound = NULL;
	gameOverTheme = NULL;
	shipMoveSound = NULL;
	enemyShipMoveSound = NULL;
	playerShootSound = NULL;
	stageTheme = NULL;
	powerUpSound = NULL;
	gainLifeSound = NULL;
	laserSound = NULL;
	enemyDeathSound = NULL;
	playerHitSound = NULL;
	enemyHitSound = NULL;
	enemyShootSound = NULL;
	playerDeathSound = NULL;
	levelCompleteSound = NULL;
	menuPickSound = NULL;
	menuSwitchSound = NULL;
	highScoreSound = NULL;
}