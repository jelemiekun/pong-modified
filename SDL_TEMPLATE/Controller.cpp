#include "Controller.h"
#include <iostream>

static int controllerCount = 0;

Controller::Controller() : mGameController(nullptr), leftYDir(0), controllerAngle(0.0), 
						movementSpeed(0), flags(new Flags) {}

Controller::~Controller() {}

void Controller::init() {
	mGameController = SDL_GameControllerOpen(controllerCount);

	if (mGameController == nullptr) {
		std::cout << "Warning: Unable to open game controller1: " << SDL_GetError() << '\n';
		flags->controllerNullptr = 1;
	} else {
		std::cout << "Controller " << controllerCount << " opened successfully." << '\n';
		++controllerCount;
		flags->controllerNullptr = 0;
	}
}

void Controller::input() {
	if (mGameController != nullptr) {
		leftYDir = SDL_GameControllerGetAxis(mGameController, SDL_CONTROLLER_AXIS_LEFTY);

		// Check if the joystick input is within the dead zone
		if (std::abs(leftYDir) < DEAD_ZONE) {
			// If within the dead zone, reset movement and angle
			leftYDir = 0;
			controllerAngle = 0;
		} else {
			int magnitude = std::abs(leftYDir);

			// Print or use magnitude and direction
			std::cout << "Magnitude of Y-axis: " << magnitude << '\n';

			if (leftYDir < 0) {
				// moving up
			} else if (leftYDir > 0) {
				// moving down
			}
		}
	}
}

SDL_GameController* Controller::getGameController() const {
	return mGameController;
}

void Controller::setGameController(SDL_GameController* controller) {
	mGameController = controller;
}

int Controller::getLeftYDir() const {
	return leftYDir;
}

int Controller::getMovementSpeed() const {
	return movementSpeed;
}


const bool& Controller::isControllerNullptr() const {
	return flags->controllerNullptr;
}