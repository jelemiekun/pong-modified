#include "TextureManager.h"
#include "SDL_image.h"
#include <iostream>

SDL_Texture* TextureManager::loadTexure(SDL_Renderer*& renderer, const char* fileName) {
	SDL_Surface* newSurface = IMG_Load(fileName);

	if (newSurface == nullptr) {
		std::cout << "Failed to load image: " << IMG_GetError() << '\n';
		SDL_FreeSurface(newSurface);
		return nullptr;
	} else {
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, newSurface);
		SDL_FreeSurface(newSurface);

		if (texture == nullptr) {
			std::cout << "Failed to create texture from surface: " << SDL_GetError() << '\n';
			return nullptr;
		} else {
			std::cout << "Texture created from surface." << '\n';
			return texture;
		}
	}
}