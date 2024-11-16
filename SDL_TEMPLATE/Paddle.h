#pragma once
#include "Object.h"

class Paddle : public Object {
public:
	Paddle();
	~Paddle();

	void moveOnOwn();
};

