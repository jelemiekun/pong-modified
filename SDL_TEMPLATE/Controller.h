#pragma once
#include "SDL.h"
class Controller {
private:
	struct Flags {
		unsigned int controllerNullptr : 1;
	};

	SDL_GameController* mGameController;
	int leftXDir;
	int leftYDir;
	constexpr static int DEAD_ZONE = 2000;
	Flags* flags;

public:
	double controllerAngle;
	float magnitude;


public:
	Controller();
	~Controller();

	void init();
	void inputXYDir();

	SDL_GameController* getGameController() const;
	void setGameController(SDL_GameController* controller);
	int getLeftYDir() const;
	const bool& isControllerNullptr() const;
};

