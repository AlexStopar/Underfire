#ifndef TEXTURE_H
#define TEXTURE_H
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

class Texture
{
public:
	Texture();
	~Texture();
	bool loadFromFile(std::string path, SDL_Renderer* renderer);
	void free();
	void render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip = NULL,
		double angle = 0.0, SDL_Point* center = NULL,
		SDL_RendererFlip flip = SDL_FLIP_NONE);
	int getWidth();
	int getHeight();
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* gameFont);
	void setWidth(int w);
	void setHeight(int h);
	SDL_Texture* pTexture;
private:
	int pWidth;
	int pHeight;
};

#endif