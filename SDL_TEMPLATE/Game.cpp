#include "Game.h"
#include "SDL_image.h"
#include <iostream>

Game::Game() : gWindow(nullptr), gRenderer(nullptr), gGameController1(nullptr),
				gGameController2(nullptr), gCursor(nullptr), acceptKeyboardInput(false),
				running(false) {

}

Game::~Game() {}

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Failed to init SDL subsystems: " << SDL_GetError() << '\n';
	} else {
		std::cout << "Initialized SDL subsystems." << '\n';
	}

	gWindow = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (gWindow == nullptr) {
		std::cout << "Failed to  create window: " << SDL_GetError() << '\n';
	} else {
		std::cout << "Window created." << '\n';
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	if (gRenderer == nullptr) {
		std::cout << "Failed to  create renderer: " << SDL_GetError() << '\n';
	} else {
		std::cout << "Renderer created." << '\n';
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Warning: SDL_Image could not initialize: " << IMG_GetError() << '\n';
	} else {
		std::cout << "SDL_Image initialized." << '\n';
	}

	int numControllers = SDL_NumJoysticks();

	if (numControllers < 0) {
		std::cout << "Warning: No controllers found. Keyboard input will be enabled" << '\n';
		acceptKeyboardInput = true;
	} else if (numControllers == 1) {
		std::cout << "Warning: Only 1 controller found. Keyboard input will be enabled." << '\n';
		acceptKeyboardInput = true;
	} else if (numControllers >= 2) {
		std::cout << "2 Controllers found." << '\n';

		gGameController1 = SDL_GameControllerOpen(0);

		if (gGameController1 == nullptr) {
			std::cout << "Warning: Unable to open game controller1: " << SDL_GetError() << '\n';
		} else {
			std::cout << "Controller 1 opened successfully." << '\n';
		}

		gGameController2 = SDL_GameControllerOpen(1);

		if (gGameController2 == nullptr) {
			std::cout << "Warning: Unable to open game controller2: " << SDL_GetError() << '\n';
		} else {
			std::cout << "Controller 2 opened successfully." << '\n';
		}
	}

	initCursor();

	running = true;
}

void Game::initCursor() {
	SDL_Surface* cursorSurface = IMG_Load("assets/cursor.png");

	if (cursorSurface == nullptr) {
		std::cout << "Failed to load image cursor: " << IMG_GetError() << '\n';
		return;
	} else {
		std::cout << "Imamge cursor loaded." << '\n';
	}

	gCursor = SDL_CreateColorCursor(cursorSurface, 0, 0);

	if (gCursor != gCursor) {
		std::cout << "Failed to create cursor: " << SDL_GetError() << '\n';
	} else {
		std::cout << "Cursor created." << '\n';
	}

	SDL_SetCursor(gCursor);

	SDL_FreeSurface(cursorSurface);
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			running = false;
		}
	}
}

void Game::update() {

}

void Game::render() {

}

const bool& Game::isRunning() const {
	return running;
}