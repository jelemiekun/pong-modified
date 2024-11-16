#pragma once

#include "SDL.h"

class Timer {
public:
	static Uint32 COUNTDOWN_TIMER;

private:
	Uint32 copyTimer;
	Uint32 startedTime;

public:
	bool started;

public:
	Timer();
	~Timer();

public:
	void startCountdown();
	bool isFinish();
};

