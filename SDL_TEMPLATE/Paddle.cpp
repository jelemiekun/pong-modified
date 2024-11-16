#include "Paddle.h"
#include <iostream>

uint16_t Paddle::SCREEN_HEIGHT = 0;

Paddle::Paddle() : Object() {}

Paddle::~Paddle() {}

void Paddle::move(const bool& isUp, const int& velocity) {
	if (isUp) {
		dstRect->y -= velocity;
		if (dstRect->y < 0) dstRect->y = 0;
	} else {
		dstRect->y += velocity;
		if (dstRect->y + dstRect->h > SCREEN_HEIGHT) dstRect->y = SCREEN_HEIGHT - dstRect->h;
	}
}

void Paddle::moveOnOwn() {

}