#ifndef TEXTDRAW_H
#define TEXTDRAW_H
#include <SDL.h>
#include <vector>
#include <string>
#include "common.h"
#include "texture.h"

class TextDraw
{
public:
	static TextDraw& getInstance()
	{
		static TextDraw instance;
		return instance;
	}
	std::vector<Texture> menuTexture;
	std::vector<Texture> highScoresTexture;
	std::vector<Texture> menuTitleTexture;
	std::vector<Texture> instructionTexture;
	bool loadText(SDL_Renderer* renderer);
	void renderHUD(SDL_Renderer* renderer);
	void loadVectors();
	void freeVectors();
	void renderGameOver(SDL_Renderer* renderer, bool isHighScore, std::string inputText);
	void renderMenuTitle(SDL_Renderer* renderer);
	void renderPause(SDL_Renderer* renderer);
	void renderBossMeter(SDL_Renderer* renderer, int hp);
	void renderHighScoreMenu(SDL_Renderer* renderer);
	void renderMenu(SDL_Renderer* renderer, int ballPosition);
	void renderInstructions(SDL_Renderer* renderer, bool isOnHighScoreMenu);
	void reset();
	int gameOverRise = 0;
	void free();
private:
	TextDraw() {};
	int wobble = 0;
	bool isWobblingRight;
	const int WOBBLE_DEPTH = 15;
	int underPos = 0;
	int firePos = SCREEN_WIDTH;
	TextDraw(TextDraw const&);
	void operator=(TextDraw const&);
	void freeFonts();
};

#endif