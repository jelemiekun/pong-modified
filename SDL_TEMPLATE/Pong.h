#pragma once

#include "Object.h"
#include "Paddle.h"

class Pong : public Object {
public:
	static bool isClassic;

private:
	enum class DIRECTIONS {
		POSITIVE,
		NEGATIVE,
		NONE
	};

private:
	DIRECTIONS dirX;
	DIRECTIONS dirY;
	int PongyPos;

public:
	bool collidedBorder;
	bool collidedPaddle;

	bool spawned;
	bool scored;
	bool playerScored;
	SDL_Rect colliders[2];

public:
	Pong();
	~Pong();

	void scaleColliders(const double& scale) override;
	void move(const bool& isLeft, const int& velocity, Paddle*& paddleLeft, Paddle*& paddleRight);
	void moveColliders(const bool& isUp, const int& velocityX, const int& velocityY = 0) override;
	void spawn(const bool& isUpOrIsLeft, const int& SCALE);
};

