#include "Object.h"
#include <iostream>

SDL_Texture* Object::spriteSheet = nullptr;
SDL_Renderer* Object::renderer = nullptr;

Object::Object() : srcRect(nullptr), dstRect(nullptr) {}

Object::~Object() {
	delete srcRect;
	delete dstRect;
}

void Object::init(SDL_Rect* srcRect) {
	this->srcRect = new SDL_Rect;
	this->srcRect->x = srcRect->x;
	this->srcRect->y = srcRect->y;
	this->srcRect->w = srcRect->w;
	this->srcRect->h = srcRect->h;

	dstRect = new SDL_Rect;
	dstRect->x = 0;
	dstRect->y = 0;
	dstRect->w = this->srcRect->w;
	dstRect->h = this->srcRect->h;
}


void Object::scaleDstRect(const double& scale) {
	dstRect->w = this->srcRect->w;
	dstRect->h = this->srcRect->h;

	dstRect->w *= scale;
	dstRect->h *= scale;
}

void Object::xPos(const int x) {
	dstRect->x = x;
}

void Object::yPos(const int y) {
	dstRect->y = y;
}

int Object::xPos() const {
	return dstRect->x;
}

int Object::yPos() const {
	return dstRect->y;
}

int Object::w() const {
	return dstRect->w;
}

int Object::h() const {
	return dstRect->h;
}

void Object::render() const {
	SDL_RenderCopy(renderer, spriteSheet, srcRect, dstRect);
}