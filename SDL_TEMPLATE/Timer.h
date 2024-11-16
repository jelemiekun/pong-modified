#pragma once

#include "SDL.h"

class Timer {
private:
	Uint32 COUNTDOWN_TIMER;
	Uint32 copyTimer;
	Uint32 startedTime;

public:
	bool started;

public:
	Timer();
	~Timer();

public:
	void setTimer(Uint16 time);
	void startCountdown();
	bool isFinish();
};

