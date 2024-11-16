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
    if (mGameController != nullptr) { // Check if the game controller is connected
        // Retrieve the current axis values for the left joystick (X and Y directions)
        leftXDir = SDL_GameControllerGetAxis(mGameController, SDL_CONTROLLER_AXIS_LEFTX);
        leftYDir = SDL_GameControllerGetAxis(mGameController, SDL_CONTROLLER_AXIS_LEFTY);

        // Calculate the magnitude of the joystick input
        magnitude = std::sqrt(static_cast<double>(leftXDir * leftXDir + leftYDir * leftYDir));

        const double maxMagnitude = std::sqrt(32767.0 * 32767.0 + 32767.0 * 32767.0);
        magnitude = magnitude / maxMagnitude; // Normalize to [0.0, 1.0]

        // Check if the joystick input is within the dead zone
        if (std::abs(leftXDir) < DEAD_ZONE && std::abs(leftYDir) < DEAD_ZONE) {
            // If within the dead zone, reset movement and angle
            leftXDir = 0;
            leftYDir = 0;
            controllerAngle = 0;
        } else {
            // Calculate the angle based on the joystick position using atan2
            controllerAngle = atan2(static_cast<double>(-leftYDir), static_cast<double>(leftXDir)) * (180.0 / M_PI);

            
        }

        //// If joystick is moved outside the dead zone, adjust the player's position
        //if (leftXDir != 0 || leftYDir != 0) {
        //    // Check if X-axis movement is outside the dead zone
        //    if (std::abs(leftXDir) > DEAD_ZONE) {
        //        // Move the player horizontally based on the joystick direction
        //        if (leftXDir > 0) {
        //            playerDot.x += movementSpeed; // Move right
        //        } else {
        //            playerDot.x -= movementSpeed; // Move left
        //        }
        //    }

        //    // Check if Y-axis movement is outside the dead zone
        //    if (std::abs(leftYDir) > DEAD_ZONE) {
        //        // Move the player vertically based on the joystick direction
        //        if (leftYDir > 0) {
        //            playerDot.y += movementSpeed; // Move down
        //        } else {
        //            playerDot.y -= movementSpeed; // Move up
        //        }
        //    }
        //}
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

const bool& Controller::isControllerNullptr() const {
	return flags->controllerNullptr;
}