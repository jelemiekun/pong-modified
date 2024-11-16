#include "Controller.h"
#include <iostream>

static int controllerCount = 0;

Controller::Controller() : mGameController(nullptr), leftYDir(0), controllerAngle(0.0), 
						magnitude(0), flags(new Flags) {}

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

void Controller::inputXYDir() {
    if (mGameController != nullptr) {
        leftXDir = SDL_GameControllerGetAxis(mGameController, SDL_CONTROLLER_AXIS_LEFTX);
        leftYDir = SDL_GameControllerGetAxis(mGameController, SDL_CONTROLLER_AXIS_LEFTY);

        magnitude = std::sqrt(static_cast<double>(leftXDir * leftXDir + leftYDir * leftYDir));

        const double maxMagnitude = std::sqrt(32767.0 * 32767.0 + 32767.0 * 32767.0);
        magnitude = magnitude / maxMagnitude;

        if (std::abs(leftXDir) < DEAD_ZONE && std::abs(leftYDir) < DEAD_ZONE) {
            leftXDir = 0;
            leftYDir = 0;
            controllerAngle = 0;
        } else {
            controllerAngle = atan2(static_cast<double>(-leftYDir), static_cast<double>(leftXDir)) * (180.0 / M_PI);
        }
    }
}

SDL_GameController* Controller::getGameController() const {
	return mGameController;
}

void Controller::setGameController(SDL_GameController* controller) {
	mGameController = controller;
}

const bool& Controller::isControllerNullptr() const {
	return flags->controllerNullptr;
}