#pragma once
#include "SDL.h"
#include <iostream>
#include "SDL_ttf.h"

class Text {
private:
	SDL_Texture* mTexture;

public:
	Text();
	~Text();

	void loadFromRenderedText(SDL_Renderer*& renderer, TTF_Font*& gFont, const std::string& text, const SDL_Rect* dstRect, const bool& isTransparent = false);
	void close();
};

