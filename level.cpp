#include <SDL.h>
#include "level.h"
#include <random>
#include <vector>
#include "common.h"
#include "texturestorage.h"
#include "soundplayer.h"
#include "player.h"
#include "highscore.h"
#include "bullet.h"
#include "enemy.h"
#include "textdraw.h"
#include "asteroid.h"
#include "gate.h"
#include "boss.h"
#include "laser.h"
#include "powerup.h"
#include "parser.h"

int scrollOffset = 0;
int frameCount = 0;
std::vector<Bullet> bullets;
std::vector<PowerUp> powerups;
std::vector<Enemy> enemies;
std::vector<Gate> gates;
std::vector<Laser> lasers;
std::vector<Asteroid> asteroids;
std::vector<Asteroid> tempAsteroids;
std::vector<Bullet> tempBullets;
int astroBreakCount = 0;
bool isPaused = false;
const int STARTING_LIFE_COUNT = 3;
bool isWon = false;

bool loadMedia(SDL_Renderer* renderer, bool isReloading)
{
	if (!TextureStorage::getInstance().loadTextures(renderer)) return false;
 	SoundPlayer::getInstance().loadSounds();
	if (isReloading) return true;
	TextDraw::getInstance().loadText(renderer);
	return true;
}

void playLevel(SDL_Renderer* renderer, SDL_Window* window)
{
	bool isScoreRecording = false;
	bool isHighScore = false;
	std::string inputText = "";

	SDL_Event e;
	Player player(PLAYER_WIDTH, SCREEN_HEIGHT / 2, PLAYER_HIT_WIDTH, PLAYER_HIT_HEIGHT, PLAYER_VEL);
	Boss boss(0, 0, BOSS_WIDTH, BOSS_HEIGHT, BOSS_VEL);
	Parser::getInstance().Parse(resolveLevelFile(), powerups, enemies, asteroids, gates, boss);

	SoundPlayer::getInstance().playMusic(player.isDead, false);
	
	while (!isQuitting)
	{
		if (isWon || player.isDead)
		{
			if (!isScoreRecording)
			{
				isScoreRecording = HighScore::getInstance().checkAndAddNewScore(score);
				if (isScoreRecording)
				{
					SoundPlayer::getInstance().playHighScore();
					isHighScore = true;
				}
			}
		}
		if (!isWon && !player.isDead && boss.isDead && !boss.isOnScreen)
		{
			SoundPlayer::getInstance().stopMusic();
			if(!isHighScore) SoundPlayer::getInstance().playWin();
			isWon = true;
		}
	
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q)
			{
				isResetting = false;
				isQuitting = false;
				reset();
				return;
			}
			if (!isPaused) SDL_EventState(SDL_KEYUP, SDL_ENABLE);
			if (e.type == SDL_QUIT)
			{
				isQuitting = true;
				isResetting = false;
				reset();
			}
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) toggleFullscreen(window);
			else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && !player.isDead && !isWon)
			{
				SoundPlayer::getInstance().pauseMusic();
				SoundPlayer::getInstance().playPause();
				if (isPaused) isPaused = false;
				else
				{
					isPaused = true;
					SDL_EventState(SDL_KEYUP, SDL_IGNORE);
				}
			}
			if (isPaused) SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
			if (!player.isDead)
			{
				if (!isPaused) player.handleEvent(e);
			}
			if (player.isDead || isWon)
			{
				if (e.type == SDL_TEXTINPUT)
				{
					if (inputText.size() < 3 && e.text.text[0] != ',') inputText += e.text.text;
				}
				else if (e.type == SDL_KEYDOWN)
				{
					if (e.key.keysym.sym == SDLK_n && !isHighScore)
					{
						isResetting = false;
						isQuitting = false;
						reset();
						return;
					}
					else if (e.key.keysym.sym == SDLK_y && !isHighScore)
					{
						isResetting = true;
						if (isWon)
						{
							isContinuing = true;
							setLives = player.playerLives.getLives();
						}
						reset();
						return;
					}
					else if (e.key.keysym.sym == SDLK_BACKSPACE)
					{
						if (inputText.size() != 0) inputText.pop_back();
					}
					else if (e.key.keysym.sym == SDLK_z)
					{
						if (isHighScore) HighScore::getInstance().addInitials(inputText);
						isHighScore = false;
					}
				}
			}
			SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		if (!isPaused)
		{
			if (!isSpeedy) scrollOffset -= SCROLL_SPEED;
			else scrollOffset -= SCROLL_SPEED * SPEED_MULTIPLIER * SPEED_MULTIPLIER;
		}
		if (scrollOffset < -TextureStorage::getInstance().bgTexture.getWidth()) scrollOffset = 0;
		TextureStorage::getInstance().bgTexture.render(scrollOffset, 0, renderer);
		TextureStorage::getInstance().bgTexture.render(scrollOffset + TextureStorage::getInstance().bgTexture.getWidth(), 0, renderer);

		if (!isPaused)
		{
			if (!player.isDead) player.move(powerups, enemies, bullets, asteroids, gates, lasers, boss);
			for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
				it->move();
			}
			for (std::vector<Laser>::iterator it = lasers.begin(); it != lasers.end(); ++it) {
				it->move();
			}
			for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
				if (!player.isDead || it->isOnScreen)
				{
					if (it->getY() > player.getY()) it->move(bullets, false);
					else it->move(bullets, true);
				}
			}
			for (std::vector<PowerUp>::iterator it = powerups.begin(); it != powerups.end(); ++it) {
				if (!player.isDead || it->isOnScreen) it->move();
			}
			for (std::vector<Gate>::iterator it = gates.begin(); it != gates.end(); ++it) {
				if (!player.isDead || it->isOnScreen) it->move();
			}

			for (std::vector<Asteroid>::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
				if (!player.isDead || it->isOnScreen) it->move(bullets);
			}
			if (!player.isDead || boss.isOnScreen) boss.move(bullets, player.isDead);	
		}
		asteroids.insert(asteroids.end(), tempAsteroids.begin(), tempAsteroids.end());
		tempAsteroids.clear();
		bullets.insert(bullets.end(), tempBullets.begin(), tempBullets.end());
		tempBullets.clear();
		for (std::vector<Gate>::iterator it = gates.begin(); it != gates.end(); ++it) {
			if (it->isOnScreen) it->render(renderer);
		}
		for (std::vector<PowerUp>::iterator it = powerups.begin(); it != powerups.end(); ++it) {
			if(it->isOnScreen) it->render(renderer);
		}
		for (std::vector<Asteroid>::iterator it = asteroids.begin(); it != asteroids.end(); ++it) {
			if (it->isOnScreen) it->render(renderer);
		}
		for (std::vector<Enemy>::iterator it = enemies.begin(); it != enemies.end(); ++it) {
			if (it->isOnScreen) it->render(renderer);
		}
		for (std::vector<Bullet>::iterator it = bullets.begin(); it != bullets.end(); ++it) {
			if (it->getX() > 0) it->render(renderer);
		}
		for (std::vector<Laser>::iterator it = lasers.begin(); it != lasers.end(); ++it) {
			if (it->isOnScreen) it->render(renderer);
		}
		boss.render(renderer);
		player.render(renderer);
		
		TextDraw::getInstance().renderHUD(renderer);
		if (isPaused) TextDraw::getInstance().renderPause(renderer);
		if (player.isDead || isWon) TextDraw::getInstance().renderGameOver(renderer, isHighScore, inputText);
		SDL_RenderPresent(renderer);
		frameCount++;
		if (frameCount == DISPLAY_LIMIT) frameCount = 0;
		if (isBoss)
		{
			enemies.clear();
			asteroids.clear();
		}
	}
}

void reset()
{
	if (!isContinuing || (int)currentLevel + 1 == (int)Level::TOTAL || !isWon) 
	{
		score = 0;
		setLives = STARTING_LIFE_COUNT;
	}
	isBoss = false;
	isWon = false;
	isFirstBulletShot = false;
	isSpeedy = false;
	firstBulletDelay = 0;
	SoundPlayer::getInstance().stopMusic();
	SoundPlayer::getInstance().stopSound();
	TextDraw::getInstance().gameOverRise = 0;
	bullets.clear();
	gates.clear();
	powerups.clear();
	enemies.clear();
	lasers.clear();
	asteroids.clear();
	tempAsteroids.clear();
}

