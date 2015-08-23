#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include "texture.h"

Texture::Texture()
{
	pTexture = NULL;
	pWidth = 0;
	pHeight = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::loadFromFile(std::string path, SDL_Renderer* renderer)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	else
	{
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL) printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		else
		{
			pWidth = loadedSurface->w;
			pHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	pTexture = newTexture;
	return pTexture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if (pTexture != NULL)
	{
		SDL_DestroyTexture(pTexture);
		pTexture = NULL;
		pWidth = 0;
		pHeight = 0;
	}
}

void Texture::render(int x, int y, SDL_Renderer* renderer, SDL_Rect* clip, double angle,
	SDL_Point* center, SDL_RendererFlip flip)
{
	if (pTexture == NULL) return;
	SDL_Rect renderQuad = { x, y, pWidth, pHeight };
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(renderer, pTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return pWidth;
}

int Texture::getHeight()
{
	return pHeight;
}

bool Texture::loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* renderer, TTF_Font* gameFont)
{
	free();
	if (textureText.size() == 0) return true;
	SDL_Surface* textSurface = TTF_RenderText_Blended(gameFont, textureText.c_str(), textColor);
	SDL_LockSurface(textSurface);
	if (textSurface == NULL) printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	else
	{
		pTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (pTexture == NULL) printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		else
		{
			pWidth = textSurface->w;
			pHeight = textSurface->h;
			SDL_FreeSurface(textSurface);
		}
		SDL_UnlockSurface(textSurface);
		textSurface = NULL;
	}
	return pTexture != NULL;
}

void Texture::setWidth(int w)
{
	pWidth = w;
}

void Texture::setHeight(int h)
{
	pHeight = h;
}