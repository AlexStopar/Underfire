#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <sstream>
#include "textdraw.h"
#include "texturestorage.h"
#include "texture.h"
#include "highscore.h"
#include "soundplayer.h"
#include "common.h"

SDL_Color textColor = { 25, 25, 112 };
TTF_Font* titleFont = NULL;
TTF_Font* scoreFont = NULL;
TTF_Font* gameOverFont = NULL;
TTF_Font* wonFont = NULL;
TTF_Font* menuTitleFont = NULL;
TTF_Font* congratsFont = NULL;
TTF_Font* instructionFont = NULL;
TTF_Font* highScoreFont = NULL;
Texture gameOverTexture;
Texture titleTexture;
Texture retryTexture;
Texture bossHPTexture;
Texture wonTexture;
Texture pauseTexture;
Texture congratsTexture;
Texture inputTexture;
const int FIRE_DELAY = 10;
const int TITLE_SPEED = 5;
const int INSTRUCTION_OFFSET = 30;
bool isUnderExploded = false;
bool isFireExploded = false;

void TextDraw::loadVectors()
{
	for (int i = 0; i < 3; i++)
	{
		menuTexture.push_back(Texture());
		instructionTexture.push_back(Texture());
	}
	for (int i = 0; i < 11; i++)
	{
		highScoresTexture.push_back(Texture());
	}
	for (int i = 0; i < 2; i++)
	{
		menuTitleTexture.push_back(Texture());
	}
}

bool TextDraw::loadText(SDL_Renderer* renderer)
{
	loadVectors();
	titleFont = TTF_OpenFont("propaganda.ttf", 36);
	scoreFont = TTF_OpenFont("propaganda.ttf", 28);
	gameOverFont = TTF_OpenFont("propaganda.ttf", 42);
	wonFont = TTF_OpenFont("propaganda.ttf", 80);
	menuTitleFont = TTF_OpenFont("propaganda.ttf", 110);
	instructionFont = TTF_OpenFont("propaganda.ttf", 25);
	congratsFont = TTF_OpenFont("propaganda.ttf", 40);
	highScoreFont = TTF_OpenFont("propaganda.ttf", 52);
	SDL_Color gameOverColor = { 64, 64, 64 };
	congratsTexture.loadFromRenderedText("You got big prizes! Enter initials", textColor, renderer, congratsFont);
	gameOverTexture.loadFromRenderedText("YOU CRASHED AND BURNED...BADLY", gameOverColor, renderer, gameOverFont);
	retryTexture.loadFromRenderedText("Will you continue? Y/N", gameOverColor, renderer, titleFont);
	wonTexture.loadFromRenderedText("Wow! Nice Flying!", gameOverColor, renderer, wonFont);
	titleTexture.loadFromRenderedText("UNDERFIRE", textColor, renderer, titleFont);
	pauseTexture.loadFromRenderedText("PAUSE", textColor, renderer, titleFont);
	menuTexture[0].loadFromRenderedText("START", textColor, renderer, titleFont);
	menuTexture[1].loadFromRenderedText("HIGH SCORES", textColor, renderer, titleFont);
	menuTexture[2].loadFromRenderedText("EXIT", textColor, renderer, titleFont);
	instructionTexture[0].loadFromRenderedText("PRESS/HOLD Z TO SHOOT/SELECT, ARROW KEYS TO MOVE", textColor, renderer, instructionFont);
	instructionTexture[1].loadFromRenderedText("DESTROY ENEMIES AND GET A HIGH SCORE!", textColor, renderer, instructionFont);
	instructionTexture[2].loadFromRenderedText("PRESS Z TO EXIT THE HIGH SCORE MENU", textColor, renderer, titleFont);
	highScoresTexture[0].loadFromRenderedText("HIGH SCORES", textColor, renderer, menuTitleFont);
	menuTitleTexture[0].loadFromRenderedText("UNDER", textColor, renderer, menuTitleFont);
	menuTitleTexture[1].loadFromRenderedText("FIRE", textColor, renderer, menuTitleFont);
	return true;
}

void TextDraw::renderHUD(SDL_Renderer* renderer)
{
	titleTexture.render(0, 0, renderer);
	std::stringstream scoreText;
	scoreText.str("");
	scoreText << "Score: " << score;
	Texture scoreTexture;
	scoreTexture.loadFromRenderedText(scoreText.str(), textColor, renderer, scoreFont);
	scoreTexture.render((titleTexture.getWidth() * 3)/2, 0, renderer);
	scoreTexture.free();
}

void TextDraw::renderBossMeter(SDL_Renderer* renderer, int hp)
{
	std::stringstream hpText;
	hpText.str("");
	hpText << "BOSS LIFE: " << hp;
	bossHPTexture.free();
	bossHPTexture.loadFromRenderedText(hpText.str(), textColor, renderer, scoreFont);
	bossHPTexture.render(0, titleTexture.getHeight(), renderer);
}

void TextDraw::renderPause(SDL_Renderer* renderer)
{
	pauseTexture.render((SCREEN_WIDTH / 2) - pauseTexture.getWidth()/2, (SCREEN_HEIGHT / 2) - pauseTexture.getHeight(), renderer);
}

void TextDraw::renderGameOver(SDL_Renderer* renderer, bool isHighScore, std::string inputText)
{
	
	if (isHighScore)
	{
		SDL_Rect gameOverRect = { 0 + wobble, SCREEN_HEIGHT - gameOverRise,
			SCREEN_WIDTH, congratsTexture.getHeight() + retryTexture.getHeight() };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xB2, 0x66, 0xFF);
		SDL_RenderFillRect(renderer, &gameOverRect);
		congratsTexture.render(wobble, SCREEN_HEIGHT - gameOverRise, renderer);
		inputTexture.loadFromRenderedText(inputText, textColor, renderer, titleFont);
		inputTexture.render((SCREEN_WIDTH / 2 - PLAYER_WIDTH) + wobble, SCREEN_HEIGHT - gameOverRise + congratsTexture.getHeight(), renderer);
	}
	else if (!isWon)
	{
		SDL_Rect gameOverRect = { 0 + wobble, SCREEN_HEIGHT - gameOverRise,
			SCREEN_WIDTH, gameOverTexture.getHeight() + retryTexture.getHeight() };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xB2, 0x66, 0xFF);
		SDL_RenderFillRect(renderer, &gameOverRect);
		gameOverTexture.render(PLAYER_HIT_WIDTH + wobble, SCREEN_HEIGHT - gameOverRise, renderer);
		retryTexture.render((SCREEN_WIDTH / 5) + wobble, SCREEN_HEIGHT - gameOverRise + gameOverTexture.getHeight(), renderer);
	}
	else
	{
		SDL_Rect gameOverRect = { 0 + wobble, SCREEN_HEIGHT - gameOverRise,
			SCREEN_WIDTH, wonTexture.getHeight() + retryTexture.getHeight() };
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xB2, 0x66, 0xFF);
		SDL_RenderFillRect(renderer, &gameOverRect);
		wonTexture.render(PLAYER_HIT_WIDTH + wobble, SCREEN_HEIGHT - gameOverRise, renderer);
		retryTexture.render((SCREEN_WIDTH / 5) + wobble, SCREEN_HEIGHT - gameOverRise + wonTexture.getHeight(), renderer);
	}
	if (gameOverRise != (SCREEN_HEIGHT / 3) + SCREEN_HEIGHT/4) gameOverRise += 2;
	if (isWobblingRight) wobble++;
	else wobble--;
	if (isWobblingRight && wobble == WOBBLE_DEPTH) isWobblingRight = false;
	else if (!isWobblingRight && wobble == -WOBBLE_DEPTH) isWobblingRight = true;
}

void TextDraw::renderMenuTitle(SDL_Renderer* renderer)
{
	if (underPos < SCREEN_WIDTH / 10) underPos += TITLE_SPEED;
	if (firePos > SCREEN_WIDTH/2 + SCREEN_WIDTH/11 && underPos > FIRE_DELAY) firePos -= TITLE_SPEED;
	if (underPos >= SCREEN_WIDTH / 10 && !isUnderExploded)
	{
		isUnderExploded = true;
		SoundPlayer::getInstance().playDeath(true);
	}
	if (firePos <= SCREEN_WIDTH / 2 + SCREEN_WIDTH / 11 && !isFireExploded)
	{
		isFireExploded = true;
		SoundPlayer::getInstance().playDeath(true);
	}
	menuTitleTexture[0].render(underPos, SCREEN_HEIGHT / 3, renderer);
	menuTitleTexture[1].render(firePos, SCREEN_HEIGHT / 3, renderer);
}

void TextDraw::renderMenu(SDL_Renderer* renderer, int ballPosition)
{
	for (size_t i = 0; i < menuTexture.size(); i++)
	{
		menuTexture[i].render(SCREEN_WIDTH / 3, (SCREEN_HEIGHT*6/10) + (i*SCREEN_HEIGHT/15), renderer);
	}
	TextureStorage::getInstance().playerTexture.render((SCREEN_WIDTH / 3) - 
		TextureStorage::getInstance().playerTexture.getWidth(), (SCREEN_HEIGHT * 6 / 10) + (ballPosition*SCREEN_HEIGHT / 15), renderer);
}

void TextDraw::renderInstructions(SDL_Renderer* renderer, bool isOnHighScoreMenu)
{
	if (isOnHighScoreMenu) instructionTexture[2].render(0, SCREEN_HEIGHT - INSTRUCTION_OFFSET, renderer);
	else
	{
		instructionTexture[1].render(SCREEN_WIDTH/7, SCREEN_HEIGHT - INSTRUCTION_OFFSET, renderer);
		instructionTexture[0].render(0, SCREEN_HEIGHT - instructionTexture[1].getHeight() - INSTRUCTION_OFFSET, renderer);
	}
}

void TextDraw::renderHighScoreMenu(SDL_Renderer* renderer)
{
	highScoresTexture[0].render(PLAYER_WIDTH/5, 0, renderer);
	for (int i = 1; i < 11; i++)
	{
		highScoresTexture[i].loadFromRenderedText(std::to_string(i) + 
			". " + HighScore::getInstance().getScores()[i - 1].first +
			" " + std::to_string(HighScore::getInstance().getScores()[i - 1].second), textColor, renderer, highScoreFont);
	}
	int startingPoint = highScoresTexture[0].getHeight();
	for (int i = 1; i < 11; i++)
	{
		highScoresTexture[i].render(SCREEN_WIDTH/4, startingPoint, renderer);
		startingPoint += highScoresTexture[1].getHeight();
	}
}

void TextDraw::free()
{
	titleTexture.free();
	pauseTexture.free();
	gameOverTexture.free();
	bossHPTexture.free();
	retryTexture.free();
	freeFonts();
	freeVectors();
}

void TextDraw::freeVectors()
{
	for (std::vector<Texture>::iterator it = menuTexture.begin(); it != menuTexture.end(); ++it) {
		it->free();
	}
	for (std::vector<Texture>::iterator it = highScoresTexture.begin(); it != highScoresTexture.end(); ++it) {
		it->free();
	}
	for (std::vector<Texture>::iterator it = menuTitleTexture.begin(); it != menuTitleTexture.end(); ++it) {
		it->free();
	}
	for (std::vector<Texture>::iterator it = instructionTexture.begin(); it != instructionTexture.end(); ++it) {
		it->free();
	}
}

void TextDraw::freeFonts()
{
	TTF_CloseFont(scoreFont);
	TTF_CloseFont(titleFont);
	TTF_CloseFont(gameOverFont);
	TTF_CloseFont(wonFont);
	TTF_CloseFont(congratsFont);
	TTF_CloseFont(highScoreFont);
	highScoreFont = NULL;
	congratsFont = NULL;
	wonFont = NULL;
	gameOverFont = NULL;
	scoreFont = NULL;
	titleFont = NULL;
}

void TextDraw::reset()
{
	underPos = 0;
	firePos = SCREEN_WIDTH;
	isUnderExploded = false;
	isFireExploded = false;
}