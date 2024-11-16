#include "Timer.h"

Timer::Timer() : COUNTDOWN_TIMER(0), copyTimer(0),
				startedTime(0), started(false) {}

Timer::~Timer() {}

void Timer::setTimer(Uint16 time) {
	COUNTDOWN_TIMER = time;
	copyTimer = time;
}

void Timer::startCountdown() {
	copyTimer = COUNTDOWN_TIMER;
	startedTime = SDL_GetTicks();
	started = true;
}

bool Timer::isFinish() {
	Uint32 timeElapsed = SDL_GetTicks() - startedTime;

	if (timeElapsed > COUNTDOWN_TIMER) {
		started = false;
	}

	return timeElapsed > COUNTDOWN_TIMER;
}