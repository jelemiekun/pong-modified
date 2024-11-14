#include "FPSmanager.h"

void FPSmanager::calculateFPS(uint16_t& countFrame, Uint32& startTime) {
	countFrame++;
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - startTime > 1000) {
		float fps = countFrame / ((currentTime - startTime) / 1000);
		countFrame = 0;
		startTime = 0;
	}
}

void FPSmanager::limitFPS(const Uint32& startTime) {
	Uint32 frameDuration = SDL_GetTicks() - startTime;

	if (frameDuration < FRAME_DURATION) {
		SDL_Delay(FRAME_DURATION - frameDuration);
	}
}