#pragma once
#include "SDL.h"
#include <iostream>
#include "SDL_ttf.h"

class Text {
public:
	static TTF_Font* gFont;

private:
	SDL_Texture* mTexture;

public:
	Text();
	~Text();

	void loadFromRenderedText(SDL_Renderer*& renderer, const std::string& text, const SDL_Rect* dstRect, const bool& isTransparent = false);
	void close();
};

