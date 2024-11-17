#pragma once
#include "Object.h"

class Paddle : public Object {
public:
	SDL_Rect colliders[3];

public:
	Paddle();
	~Paddle();

	void scaleColliders(const double& scale) override;
	void move(const bool& isUp, const int& velocity);
	void moveColliders(const bool& isUp, const int& velocityY, const int& velocityX = 0) override;
	void moveOnOwn(const int &limit, const bool &isRight,const int& pongXpos, const int &pongYpos, const int& velocity);
};