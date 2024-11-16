#pragma once

#include "Object.h"

class Pong : public Object {
public:
	static uint16_t SCREEN_WIDTH;
	static uint16_t SCREEN_HEIGHT;
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
	bool spawned;

public:
	Pong();
	~Pong();

	void move(const bool& isUp, const int& velocity) override;
	void spawn(const bool& isLeft);
};

