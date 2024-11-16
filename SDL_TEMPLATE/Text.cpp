#include "Text.h"

Text::Text() : mTexture(nullptr) {}

TTF_Font* Text::gFont = nullptr;

void Text::loadFromRenderedText(SDL_Renderer*& renderer, const std::string& text, const SDL_Rect* dstRect, const bool& isTransparent) {
	if (mTexture != nullptr) SDL_DestroyTexture(mTexture);
	mTexture = nullptr;

	SDL_Color white = { 255, 255, 255, 255 };
	if (isTransparent) white.a = 90;
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, text.c_str(), white);

	if (textSurface == nullptr) {
		std::cout << "Unable to render text surface: " << TTF_GetError() << '\n';
		return;
	}

	mTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	if (mTexture == nullptr) {
		std::cout << "Failed to create texture from rendered text: " << SDL_GetError() << '\n';
		return;
	}

	SDL_FreeSurface(textSurface);

	SDL_RenderCopy(renderer, mTexture, nullptr, dstRect);
}