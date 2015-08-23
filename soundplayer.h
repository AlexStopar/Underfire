#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H
#include "powerup.h"

class SoundPlayer
{
public:
	static SoundPlayer& getInstance()
	{
		static SoundPlayer instance;
		return instance;
	}
	bool loadSounds();
	void playMusic(bool isDead, bool isBoss);
	void pauseMusic();
	void stopMusic();
	void stopSound();
	void playPause();
	void playWin();
	void playHighScore();
	void playMenuPick();
	void playMenuSwitch();
	void playGateTrigger();
	void playLaser();
	void playHit(bool isEnemy);
	void playDeath(bool isEnemy);
	void playCoupleMoreShots();
	void playShoot(bool isEnemy);
	void playShipMove(bool isEnemy);
	void playPowerUp(PowerUpType pt);
	void free();
private:
	SoundPlayer() {};
	const int MOVE_VOLUME = 15;
	const int SHOOT_VOLUME = 30;
	SoundPlayer(SoundPlayer const&);
	void operator=(SoundPlayer const&);
};

#endif