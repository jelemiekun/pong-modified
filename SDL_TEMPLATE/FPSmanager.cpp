#include "FPSmanager.h"
#include <iostream>

void FPSmanager::calculateFPS(uint16_t& countFrame, Uint32& startTime) {
	countFrame++;
	Uint32 currentTime = SDL_GetTicks();

	if (currentTime - startTime > 1000) {
		float fps = static_cast<float>(countFrame) / ((currentTime - startTime) / 1000.0f);
		std::cout << "FPS: " << fps << '\n';
		countFrame = 0;
		startTime = currentTime;
	}
}

void FPSmanager::limitFPS(const Uint32& startTime) {
	Uint32 frameDuration = SDL_GetTicks() - startTime;

	if (frameDuration < FRAME_DURATION) {
		SDL_Delay(FRAME_DURATION - frameDuration);
	}
}