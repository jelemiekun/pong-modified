#pragma once
#include "Object.h"

class Paddle : public Object {
public:
	static uint16_t SCREEN_HEIGHT;

public:
	Paddle();
	~Paddle();

	void move(const bool& isUp, const int& velocity) override;
	void moveOnOwn();
};

