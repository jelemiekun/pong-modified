#include "SFX.h"
#include <iostream>

SFX::SFX() : SFXborder(nullptr), SFXpaddle(nullptr), SFXscored(nullptr) {}

SFX::~SFX() {}

void SFX::initSFXs() {
	SFXborder = Mix_LoadWAV("assets/border.wav");
	if (SFXborder == nullptr) std::cout << "Failed to load SFX border: " << Mix_GetError() << '\n';
	else std::cout << "SFX border loaded." << '\n';

	SFXpaddle = Mix_LoadWAV("assets/hit.wav");
	if (SFXpaddle == nullptr) std::cout << "Failed to load SFX paddle: " << Mix_GetError() << '\n';
	else std::cout << "SFX paddle loaded." << '\n';

	SFXscored = Mix_LoadWAV("assets/scored.wav");
	if (SFXscored == nullptr) std::cout << "Failed to load SFX scored: " << Mix_GetError() << '\n';
	else std::cout << "SFX scored loaded." << '\n';
}

void SFX::playSFX(SOUNDS sfx) {
	switch (sfx) {
	case SOUNDS::border: Mix_PlayChannel(-1, SFXborder, 0); break;
	case SOUNDS::paddle: Mix_PlayChannel(-1, SFXpaddle, 0); break;
	case SOUNDS::scored: Mix_PlayChannel(-1, SFXscored, 0); break;
	default: break;
	}
}