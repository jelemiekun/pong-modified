#pragma once

#include "SDL.h"

class Object {
public:
	static SDL_Texture* spriteSheet;
	static SDL_Renderer* renderer;

public:
	static uint16_t SCREEN_WIDTH;
	static uint16_t SCREEN_HEIGHT;

protected:
	SDL_Rect* srcRect;
	SDL_Rect* dstRect;

protected:
	Object();
	~Object();

public:
	void init(SDL_Rect* srcRect);
	void scaleDstRect(const double& scale);
	virtual void scaleColliders(const double& scale) = 0;
	virtual void moveColliders(const bool& isUp, const int& velocityX, const int& velocityY = 0) = 0;
	void xPos(const int x);
	void yPos(const int y);
	int xPos() const;
	int yPos() const;
	int w() const;
	int h() const;
	void render() const;
};