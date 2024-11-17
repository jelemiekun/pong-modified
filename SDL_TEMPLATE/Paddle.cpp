#include "Paddle.h"
#include <iostream>

Paddle::Paddle() : Object() {}

Paddle::~Paddle() {}

void Paddle::scaleColliders(const double& scale) {
	for (int i = 0; i < std::size(colliders); i++) {
		colliders[i].w *= scale;
		colliders[i].h *= scale;
	}
}

void Paddle::move(const bool& isUp, const int& velocity) {
    int actualVelocity = velocity;

    if (isUp) {
        if (dstRect->y < velocity) actualVelocity = dstRect->y;
        dstRect->y -= actualVelocity;

    } else {
        if (dstRect->y + dstRect->h + velocity > SCREEN_HEIGHT)
            actualVelocity = SCREEN_HEIGHT - (dstRect->y + dstRect->h);

        dstRect->y += actualVelocity;
    }

    if (actualVelocity > 0) moveColliders(isUp, actualVelocity);
}

void Paddle::moveColliders(const bool& isUp, const int& velocityY, const int& velocityX) {
	for (int i = 0; i < std::size(colliders); i++) {
		if (isUp) colliders[i].y -= velocityY;
		else colliders[i].y += velocityY;
	}
}

void Paddle::moveOnOwn(const int& limit, const bool &isRight, const int& pongXpos, const int& pongYpos, const int& maxVelocity) {
    int center = dstRect->y + (dstRect->h / 2);
    int botCenterOffset = 200;
    int distance = pongYpos - center;

    int velocity = std::min(abs(distance), maxVelocity);

    if ((isRight && (pongXpos > (limit + botCenterOffset))) || (!isRight && (pongXpos < (limit - botCenterOffset)))) {
        if (distance > 0) move(false, maxVelocity / 2);
        else if (distance < 0) move(true, maxVelocity /2);
    }
}
