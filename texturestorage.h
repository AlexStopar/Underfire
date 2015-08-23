#ifndef TEXTURESTORAGE_H
#define TEXTURESTORAGE_H
#include "texture.h"
#include <SDL.h>


class TextureStorage
{
public:
	static TextureStorage& getInstance()
	{
		static TextureStorage instance;
		return instance;
	}
	Texture bgTexture;
	Texture playerTexture;
	Texture bulletTexture;
	Texture enemyBulletTexture;
	Texture powerUpTexture;
	Texture enemyEasyTexture;
	Texture enemyMediumTexture;
	Texture explosionTexture;
	Texture asteroidTexture;
	Texture bossTexture;
	Texture gateTexture;
	Texture laserTexture;
	bool loadTextures(SDL_Renderer* renderer);
	void free();
private:
	TextureStorage() {};
	TextureStorage(TextureStorage const&);
	void operator=(TextureStorage const&);
};


#endif