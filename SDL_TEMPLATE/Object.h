#pragma once

#include "SDL.h"

class Object {
public:
	static SDL_Texture* spriteSheet;
	static SDL_Renderer* renderer;

private:
	SDL_Rect* srcRect;
	SDL_Rect* dstRect;

protected:
	Object();
	~Object();

public:
	void init(SDL_Rect* srcRect);
	void scaleDstRect(const double& scale);
	void move(const bool& isUp, const int& velocity);
	void xPos(const int x);
	void yPos(const int y);
	int xPos() const;
	int yPos() const;
	int w() const;
	int h() const;
	void render() const;
};