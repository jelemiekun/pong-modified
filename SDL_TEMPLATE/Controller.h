#pragma once
#include "SDL.h"
class Controller {
private:
	struct Flags {
		unsigned int controllerNullptr : 1;
	};

	SDL_GameController* mGameController;
	int leftYDir;
	double controllerAngle;
	constexpr static int DEAD_ZONE = 5000;
	int movementSpeed;
	Flags* flags;
	

public:
	Controller();
	~Controller();

	void init();
	void input();

	SDL_GameController* getGameController() const;
	void setGameController(SDL_GameController* controller);
	int getLeftYDir() const;
	int getMovementSpeed() const;
	const bool& isControllerNullptr() const;
};

