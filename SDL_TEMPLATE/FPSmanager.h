#pragma once
#include "SDL.h"

constexpr static Uint8 FPS = 60;
constexpr static float FRAME_DURATION = 1000.0F / FPS;

namespace FPSmanager {
	void calculateFPS(uint16_t& countFrame, Uint32& startTime);

	void limitFPS(const Uint32& startTime);
};

