#include "Game.h"
#include <iostream>

Game::Game() : gWindow(nullptr), gRenderer(nullptr), gGameController1(nullptr),
				gGameController2(nullptr), gCursor(nullptr), acceptKeyboardInput(false),
				running(false), flags(nullptr), gFont(nullptr), text(nullptr),
				currentController(nullptr) {

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
	bool controllerFound = false;

	if (numControllers <= 0) {
		std::cout << "Warning: No controllers found. Keyboard input will be enabled" << '\n';
		acceptKeyboardInput = true;
		controllerFound = false;
	} else if (numControllers == 1) {
		std::cout << "Warning: Only 1 controller found. Keyboard input will be enabled." << '\n';
		acceptKeyboardInput = true;
		controllerFound = true;
		gGameController1 = new Controller;
	} else if (numControllers >= 2) {
		std::cout << "2 Controllers found." << '\n';
		acceptKeyboardInput = false;
		controllerFound = true;
		gGameController1 = new Controller;
		gGameController2 = new Controller;
	}

	if (controllerFound) {
		if (gGameController1 != nullptr) gGameController1->init();
		if (gGameController2 != nullptr) gGameController2->init();

		currentController = new Controller;
		if (gGameController1->getGameController() != nullptr) {
			currentController->setGameController(gGameController1->getGameController());
		} else {
			if (gGameController2->getGameController() != nullptr) {
				currentController->setGameController(gGameController2->getGameController());
			}
		}
	}

	if (TTF_Init() == -1) {
		std::cout << "SDL_TTF could not initialize: " << TTF_GetError() << '\n';
	} else {
		std::cout << "SDL_TTF successfully initialized." << '\n';

		gFont = TTF_OpenFont("assets/PressStart2P-vaV7.ttf", 28);

		if (gFont == nullptr) {
			std::cout << "Failed to load font: " << TTF_GetError() << '\n';
		} else {
			std::cout << "Font loaded successfully." << '\n';
			text = new Text;
		}
	}

	flags = new GameFlags;
	resetFlags();

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

void Game::resetFlags() {
	flags->inStart = 1;
	flags->inPlaying = 0;
	flags->inWin = 0;
	flags->inLose = 0;

	flags->isTwoPlayer = 0;
	flags->isClassic = 0;
	flags->isDoubleEnemy = 0;
	flags->isDoublePaddle = 0;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			running = false;
		} else {
			// If in start menu
			if (currentController != nullptr) {

			}

			if (flags->inStart) {
				switch (gEvent.type) {
				case SDL_MOUSEMOTION:
					break;
				default:
					break;
				}
			}
		}
	}
}

void Game::update() {

}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	// If in start menu
	if (flags->inStart) {
		// Main border
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 0, 0, 400, 560 };
			backgroundWhite.x = (SCREEN_WIDTH / 2) - (backgroundWhite.w / 2);
			backgroundWhite.y = (SCREEN_HEIGHT / 2) - (backgroundWhite.h / 2);
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 0, 0, backgroundWhite.w - 10, backgroundWhite.h - 10 };
			backgroundBlack.x = (SCREEN_WIDTH / 2) - (backgroundBlack.w / 2);
			backgroundBlack.y = (SCREEN_HEIGHT / 2) - (backgroundBlack.h / 2);
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 400, 110, 290, 80 };
			text->loadFromRenderedText(gRenderer, gFont, "PONG", &textDstRect);
		}
		// Players
		{
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 400, 200, 20, 20 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { backgroundWhite.x + 2, backgroundWhite.y + 2, backgroundWhite.w - 4, backgroundWhite.h - 4 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { backgroundWhite.x + 30, 202, 70, 20 };
			text->loadFromRenderedText(gRenderer, gFont, "1 PLAYER", &textDstRect);


			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			backgroundWhite = { 580, 200, 20, 20 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			backgroundBlack = { backgroundWhite.x + 2, backgroundWhite.y + 2, backgroundWhite.w - 4, backgroundWhite.h - 4 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			textDstRect = { backgroundWhite.x + 30, 202, 70, 20 };
			text->loadFromRenderedText(gRenderer, gFont, "2 PLAYERS", &textDstRect);


			// Toggle
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			backgroundWhite = { 405, 205, 10, 10 };
			if (flags->isTwoPlayer) backgroundWhite.x = 585;
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

		}
		// Classic Mode
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 0, 0, 280, 70 };
			backgroundWhite.x = (SCREEN_WIDTH / 2) - (backgroundWhite.w / 2);
			backgroundWhite.y = (SCREEN_HEIGHT / 2) - (backgroundWhite.h / 2) - 80;
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 0, 0, backgroundWhite.w - 10, backgroundWhite.h - 10};
			backgroundBlack.x = (SCREEN_WIDTH / 2) - (backgroundBlack.w / 2);
			backgroundBlack.y = (SCREEN_HEIGHT / 2) - (backgroundBlack.h / 2) - 80;
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 472, 262, 140, 40 };
			text->loadFromRenderedText(gRenderer, gFont, "CLASSIC", &textDstRect);
		}
		// Double Enemy
		{ 
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			if (flags->isTwoPlayer) SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 90);
			SDL_Rect backgroundWhite = { 0, 0, 280, 70 };
			backgroundWhite.x = (SCREEN_WIDTH / 2) - (backgroundWhite.w / 2);
			backgroundWhite.y = (SCREEN_HEIGHT / 2) - (backgroundWhite.h / 2); 
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 0, 0, backgroundWhite.w - 10, backgroundWhite.h - 10 };
			backgroundBlack.x = (SCREEN_WIDTH / 2) - (backgroundBlack.w / 2);
			backgroundBlack.y = (SCREEN_HEIGHT / 2) - (backgroundBlack.h / 2);
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 422, 342, 240, 40 };
			text->loadFromRenderedText(gRenderer, gFont, "DOUBLE ENEMY", &textDstRect, flags->isTwoPlayer);
		}
		// Double Paddle
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 0, 0, 280, 70 };
			backgroundWhite.x = (SCREEN_WIDTH / 2) - (backgroundWhite.w / 2);
			backgroundWhite.y = (SCREEN_HEIGHT / 2) - (backgroundWhite.h / 2) + 80;
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 0, 0, backgroundWhite.w - 10, backgroundWhite.h - 10 };
			backgroundBlack.x = (SCREEN_WIDTH / 2) - (backgroundBlack.w / 2);
			backgroundBlack.y = (SCREEN_HEIGHT / 2) - (backgroundBlack.h / 2) + 80;
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 422, 422, 240, 40 };
			text->loadFromRenderedText(gRenderer, gFont, "DOUBLE PADDLE", &textDstRect);
		}
		// Quit
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 0, 0, 280, 70 };
			backgroundWhite.x = (SCREEN_WIDTH / 2) - (backgroundWhite.w / 2);
			backgroundWhite.y = (SCREEN_HEIGHT / 2) - (backgroundWhite.h / 2) + 200;
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 0, 0, backgroundWhite.w - 10, backgroundWhite.h - 10 };
			backgroundBlack.x = (SCREEN_WIDTH / 2) - (backgroundBlack.w / 2);
			backgroundBlack.y = (SCREEN_HEIGHT / 2) - (backgroundBlack.h / 2) + 200;
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 494, 542, 90, 40 };
			text->loadFromRenderedText(gRenderer, gFont, "QUIT", &textDstRect);
		}
	}

	SDL_RenderPresent(gRenderer);
}

const bool& Game::isRunning() const {
	return running;
}