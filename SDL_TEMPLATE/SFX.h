#pragma once
#include "SDL_mixer.h"

enum class SOUNDS {
	border,
	paddle,
	scored
};

class SFX {
private:
	Mix_Chunk* SFXborder;
	Mix_Chunk* SFXpaddle;
	Mix_Chunk* SFXscored;

public:
	SFX();
	~SFX();

	void initSFXs();
	void playSFX(SOUNDS sfx);
};

