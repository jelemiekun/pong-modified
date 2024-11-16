#include "Game.h"
#include <iostream>
#include <memory>

Game::Game() : gWindow(nullptr), gRenderer(nullptr), gGameController1(nullptr),
				gGameController2(nullptr), gCursor(nullptr), acceptKeyboardInput(false),
				running(false), flags(nullptr), gFont(nullptr), text(nullptr),
				currentController(nullptr), leftPaddle(nullptr), centerLeftPaddle(nullptr),
				centerRightPaddle(nullptr), rightPaddle(nullptr), 
				pong1(nullptr), pong2(nullptr), timerPong1(nullptr), timerPong2(nullptr) {

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

	Object::renderer = gRenderer;

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cout << "Warning: SDL_Image could not initialize: " << IMG_GetError() << '\n';
	} else {
		std::cout << "SDL_Image initialized." << '\n';
	}

	initPaddlesAndCircle();

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
		std::cout << "2 Controllers found. Keyboard input will be disabled." << '\n';
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
			currentController = gGameController1;
		} else {
			if (gGameController2->getGameController() != nullptr) {
				currentController = gGameController2;
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

void Game::initPaddlesAndCircle() {
	Object::spriteSheet = TextureManager::loadTexure(gRenderer, "assets/spriteSheet.png");

	int w;
	int h;

	SDL_QueryTexture(Object::spriteSheet, NULL, NULL, &w, &h);

	// Initialize paddles
	{
		Paddle::SCREEN_HEIGHT = SCREEN_HEIGHT;

		SDL_Rect* rectLeftPaddle = new SDL_Rect;
		rectLeftPaddle->x = 0;
		rectLeftPaddle->y = 0;
		rectLeftPaddle->w = w / 4;
		rectLeftPaddle->h = h;

		SDL_Rect* rectRightPaddle = new SDL_Rect;
		rectRightPaddle->x = w / 4;
		rectRightPaddle->y = 0;
		rectRightPaddle->w = w / 4;
		rectRightPaddle->h = h;

		leftPaddle = new Paddle;
		centerLeftPaddle = new Paddle;
		centerRightPaddle = new Paddle;
		rightPaddle = new Paddle;

		leftPaddle->init(rectLeftPaddle);
		centerLeftPaddle->init(rectRightPaddle);
		centerRightPaddle->init(rectLeftPaddle);
		rightPaddle->init(rectRightPaddle);

		delete rectLeftPaddle;
		delete rectRightPaddle;
	}

	// Initialize pong
	{
		Pong::SCREEN_WIDTH = SCREEN_WIDTH;
		Pong::SCREEN_HEIGHT = SCREEN_HEIGHT;

		SDL_Rect* rectPong = new SDL_Rect;
		rectPong->x = (w / 8) * 4;
		rectPong->y = 0;
		rectPong->w = w / 8;
		rectPong->h = w / 4;

		pong1 = new Pong;
		pong2 = new Pong;

		pong1->init(rectPong);
		pong2->init(rectPong);

		delete rectPong;
	}

	// Initialize pong timer
	{
		timerPong1 = new Timer;
		timerPong1->setTimer(3500);

		timerPong2 = new Timer;
		timerPong2->setTimer(3500);
	}
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

	flags->clicked = 0;
	flags->dirR = 0;
	flags->dirUR = 0;
	flags->dirU = 0;
	flags->dirUL = 0;
	flags->dirL = 0;
	flags->dirDL = 0;
	flags->dirD = 0;
	flags->dirDR = 0;

	flags->dirUP2 = 0;
	flags->dirDP2 = 0;
}

void Game::drawCenterLines() {
	SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

	auto drawDashedLines = [](SDL_Renderer*& gRenderer, const int& x) {
		int y = 0;
		bool drawDash = true;
		const int DASH_LENGTH = 10;
		const int GAP_LENGTH = 10;

		while (y < SCREEN_HEIGHT) {
			if (drawDash) {
				// Draw a line segment (dash)
				SDL_RenderDrawLine(gRenderer, x, y, x, y + DASH_LENGTH);
				y += DASH_LENGTH;
			} else {
				// Skip a gap
				y += GAP_LENGTH;
			}
			drawDash = !drawDash;
		}
	};

	if (flags->isClassic) {
		drawDashedLines(gRenderer, SCREEN_WIDTH / 2);
	}
	if (flags->isDoubleEnemy || flags->isDoublePaddle) {
		SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT);

		drawDashedLines(gRenderer, (SCREEN_WIDTH / 4));
		drawDashedLines(gRenderer, (SCREEN_WIDTH / 4) * 3);
	}
}

void Game::initClassicGame() {
	const int SCALE = 6;
	const int ALLOWANCE = 50;

	leftPaddle->scaleDstRect(SCALE);
	leftPaddle->xPos(ALLOWANCE);
	leftPaddle->yPos((SCREEN_HEIGHT / 2) - (leftPaddle->h() / 2));

	rightPaddle->scaleDstRect(SCALE);
	rightPaddle->xPos(SCREEN_WIDTH - (rightPaddle->xPos() + rightPaddle->w()) - ALLOWANCE);
	rightPaddle->yPos((SCREEN_HEIGHT / 2) - (leftPaddle->h() / 2));

	pong1->scaleDstRect(SCALE / 2);

	Pong::isClassic = true;
}

void Game::initDoubleEnemyOrPaddleGame() {
	const int SCALE = 3;
	const int ALLOWANCE = 20;

	leftPaddle->scaleDstRect(SCALE);
	leftPaddle->xPos(ALLOWANCE);
	leftPaddle->yPos((SCREEN_HEIGHT / 2) - (leftPaddle->h() / 2));

	centerLeftPaddle->scaleDstRect(SCALE);
	centerLeftPaddle->xPos((SCREEN_WIDTH / 2) - centerLeftPaddle->w() - ALLOWANCE);
	centerLeftPaddle->yPos((SCREEN_HEIGHT / 2) - (centerLeftPaddle->h() / 2));

	centerRightPaddle->scaleDstRect(SCALE);
	centerRightPaddle->xPos((SCREEN_WIDTH / 2) + ALLOWANCE);
	centerRightPaddle->yPos((SCREEN_HEIGHT / 2) - (centerLeftPaddle->h() / 2));

	rightPaddle->scaleDstRect(SCALE);
	rightPaddle->xPos(SCREEN_WIDTH - (rightPaddle->xPos() + rightPaddle->w()) - ALLOWANCE);
	rightPaddle->yPos((SCREEN_HEIGHT / 2) - (rightPaddle->h() / 2));

	pong1->scaleDstRect(SCALE / 2);

	pong2->scaleDstRect(SCALE / 2);

	Pong::isClassic = false;
}

void Game::input() {
	while (SDL_PollEvent(&gEvent)) {
		if (gEvent.type == SDL_QUIT) {
			running = false;
		} else {
			
			// If in start menu
			if (flags->inStart) {
				// Handle mouse inputs in start menu
				switch (gEvent.type) {
				case SDL_MOUSEBUTTONDOWN:
					if (gEvent.button.button == SDL_BUTTON_LEFT) {
						flags->clicked = 1;
					}

					break;
				default:
					break;
				}

				// Handle controller axis inputs in start enu
				if (currentController != nullptr) {
					switch (gEvent.type) {
						// Handle motion momvement of controller
					case SDL_CONTROLLERAXISMOTION:
						currentController->inputXYDir();

						flags->dirR = currentController->controllerAngle >= -22.5 && currentController->controllerAngle < 22.5;
						flags->dirUR = currentController->controllerAngle >= 22.5 && currentController->controllerAngle < 67.5;
						flags->dirU = currentController->controllerAngle >= 67.5 && currentController->controllerAngle < 112.5;
						flags->dirUL = currentController->controllerAngle >= 112.5 && currentController->controllerAngle < 157.5;
						flags->dirL = ((currentController->controllerAngle >= 157.5 && currentController->controllerAngle <= 180) 
							|| (currentController->controllerAngle >= -180 && currentController->controllerAngle < -157.5));
						flags->dirDL = currentController->controllerAngle >= -157.5 && currentController->controllerAngle < -112.5;
						flags->dirD = currentController->controllerAngle >= -112.5 && currentController->controllerAngle < -67.5;
						flags->dirDR = currentController->controllerAngle >= -67.5 && currentController->controllerAngle < -22.5;
						break;
					case SDL_CONTROLLERBUTTONDOWN:
						if (gEvent.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
							flags->clicked = 1;
						}
					default:
						break;
					}
					
				}
			}

			// If playing
			if (flags->inPlaying) {

				// Handle first input
				{
					if (gGameController1 != nullptr && gEvent.type == SDL_CONTROLLERAXISMOTION) {
						gGameController1->inputXYDir();

						flags->dirU = gGameController1->controllerAngle >= 67.5 && gGameController1->controllerAngle < 112.5;
						flags->dirD = gGameController1->controllerAngle >= -112.5 && gGameController1->controllerAngle < -67.5;
					} else if ((gEvent.type == SDL_KEYDOWN || gEvent.type == SDL_KEYUP) && gGameController1 == nullptr) {
						// Handle key 'w' events
						if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_w) flags->dirU = 1;
						if (gEvent.type == SDL_KEYUP && gEvent.key.keysym.sym == SDLK_w) flags->dirU = 0;

						// Handle key 's' events
						if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_s) flags->dirD = 1;
						if (gEvent.type == SDL_KEYUP && gEvent.key.keysym.sym == SDLK_s) flags->dirD = 0;
					}
				}

				// If two players, handle second input
				if (flags->isTwoPlayer || flags->isDoublePaddle) {
					if (gGameController2 != nullptr && gEvent.type == SDL_CONTROLLERAXISMOTION) {
						gGameController2->inputXYDir();

						flags->dirUP2 = gGameController2->controllerAngle >= 67.5 && gGameController2->controllerAngle < 112.5;
						flags->dirDP2 = gGameController2->controllerAngle >= -112.5 && gGameController2->controllerAngle < -67.5;
					} else if (gGameController2 == nullptr && (gEvent.type == SDL_KEYDOWN || gEvent.type == SDL_KEYUP)) {
						// Handle key 'arrow up' events
						if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_UP) flags->dirUP2 = 1;
						if (gEvent.type == SDL_KEYUP && gEvent.key.keysym.sym == SDLK_UP) flags->dirUP2 = 0;

						// Handle key 'arrow down' events
						if (gEvent.type == SDL_KEYDOWN && gEvent.key.keysym.sym == SDLK_DOWN) flags->dirDP2 = 1;
						if (gEvent.type == SDL_KEYUP && gEvent.key.keysym.sym == SDLK_DOWN) flags->dirDP2 = 0;

					}
				}
			}
		}
	}
}

void Game::update() {
	// If in start menu
	if (flags->inStart) {
		int mouseX;
		int mouseY;

		SDL_GetMouseState(&mouseX, &mouseY);
		
		// Handle click event from mouse and controller
		if (flags->clicked) {
			// Toggle player modes
			{
				// Set to 1-player mode
				if (mouseX > 402 && mouseX < 500 && mouseY > 200 && mouseY < 220) { flags->isTwoPlayer = 0; }

				// Set to 2-player mode
				if (mouseX > 582 && mouseX < 680 && mouseY > 200 && mouseY < 220) { flags->isTwoPlayer = 1; }
			}

			{
				// Play classic
				if (mouseX > 405 && mouseX < 675 && mouseY > 250 && mouseY < 310) {
					flags->inStart = 0;
					flags->inPlaying = 1;
					flags->isClassic = 1;
					initClassicGame();
				}

				// Play double enemy (single player only)
				if (!flags->isTwoPlayer) {
					if (mouseX > 405 && mouseX < 675 && mouseY > 330 && mouseY < 390) {
						flags->inStart = 0;
						flags->inPlaying = 1;
						flags->isDoubleEnemy = 1;
						initDoubleEnemyOrPaddleGame();
					}
				}

				// Play double paddle
				if (mouseX > 405 && mouseX < 675 && mouseY > 410 && mouseY < 470) {
					flags->inStart = 0;
					flags->inPlaying = 1;
					flags->isDoublePaddle = 1;
					initDoubleEnemyOrPaddleGame();
				}

				// Quit
				if (mouseX > 405 && mouseX < 675 && mouseY > 530 && mouseY < 590) {
					flags->inStart = 1;
					flags->inPlaying = 0;
					running = false;
				}
			}

			flags->clicked = 0;
		}


		// Move the cursor using controller
		if (currentController != nullptr) {
			float movement = CURSOR_SPEED * currentController->magnitude;

			if (flags->dirR) SDL_WarpMouseInWindow(gWindow, mouseX + movement, mouseY);
			if (flags->dirUR) SDL_WarpMouseInWindow(gWindow, mouseX + movement, mouseY - movement);
			if (flags->dirU) SDL_WarpMouseInWindow(gWindow, mouseX, mouseY - movement);
			if (flags->dirUL) SDL_WarpMouseInWindow(gWindow, mouseX - movement, mouseY - movement);
			if (flags->dirL) SDL_WarpMouseInWindow(gWindow, mouseX - movement, mouseY);
			if (flags->dirDL) SDL_WarpMouseInWindow(gWindow, mouseX - movement, mouseY + movement);
			if (flags->dirD) SDL_WarpMouseInWindow(gWindow, mouseX, mouseY + movement);
			if (flags->dirDR) SDL_WarpMouseInWindow(gWindow, mouseX + movement, mouseY + movement);
		}
	}

	// If playing
	if (flags->inPlaying) {

		// Update paddles
		{
			if (flags->isClassic) {
				// Handle input player one
				int velocityP1 = (gGameController1 == nullptr) ? OBJECTSPEED : OBJECTSPEED * gGameController1->magnitude;

				if (flags->dirU) leftPaddle->move(true, velocityP1);
				if (flags->dirD) leftPaddle->move(false, velocityP1);

				// Handle input player two
				if (flags->isTwoPlayer) {
					int velocityP2 = (gGameController2 == nullptr) ? OBJECTSPEED : OBJECTSPEED * gGameController2->magnitude;

					if (flags->dirUP2) rightPaddle->move(true, velocityP2);
					if (flags->dirDP2) rightPaddle->move(false, velocityP2);
				} else { // Move the paddle on its own if two players are not playing
					rightPaddle->moveOnOwn();
				}
			}

			if (flags->isDoubleEnemy || flags->isDoublePaddle) {
				// Handle input player one
				int velocityP1 = (gGameController1 == nullptr) ? OBJECTSPEED : OBJECTSPEED * gGameController1->magnitude;

				// Move the center left paddle
				if (flags->dirU) centerLeftPaddle->move(true, velocityP1);
				if (flags->dirD) centerLeftPaddle->move(false, velocityP1);

				// If double enemy, move also the center right paddle
				if (flags->isDoubleEnemy) {
					if (flags->dirU) centerRightPaddle->move(true, velocityP1);
					if (flags->dirD) centerRightPaddle->move(false, velocityP1);
				}

				// If double paddle, handle second input
				if (flags->isDoublePaddle) {
					int velocityP2 = (gGameController2 == nullptr) ? OBJECTSPEED : OBJECTSPEED * gGameController2->magnitude;

					if (flags->dirUP2) centerRightPaddle->move(true, velocityP2);
					if (flags->dirDP2) centerRightPaddle->move(false, velocityP2);
				}

				// Then move on own the left and right paddle
				leftPaddle->moveOnOwn();
				rightPaddle->moveOnOwn();
			}
		}
		
		// Update pong(s)
		{
			if (!pong1->spawned && !timerPong1->started) {
				timerPong1->startCountdown();
			} else if (!pong1->spawned && timerPong1->started && timerPong1->isFinish()) {
				pong1->spawn(true);
			} else if (pong1->spawned) {
				pong1->move(true, PONG_SPEED);
			}

			// If double enemy or double paddle, update also the second pong
			if (flags->isDoubleEnemy || flags->isDoublePaddle) {
				if (!pong2->spawned && !timerPong2->started) {
					timerPong2->startCountdown();
				} else if (!pong2->spawned && timerPong2->started && timerPong2->isFinish()) {
					pong2->spawn(false);
				} else if (pong2->spawned) {
					pong2->move(true, PONG_SPEED);
				}
			}
		}
	}
}

void Game::render() {
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);

	// If in start menu
	if (flags->inStart) {
		SDL_ShowCursor(SDL_ENABLE);
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
			// White background for "1 PLAYER" option
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 400, 200, 20, 20 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			// Black inner rectangle for "1 PLAYER" option
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 402, 202, 16, 16 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			// Text destination for "1 PLAYER"
			SDL_Rect textDstRect = { 430, 202, 70, 20 };
			text->loadFromRenderedText(gRenderer, gFont, "1 PLAYER", &textDstRect);


			// White background for "2 PLAYERS" option
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			backgroundWhite = { 580, 200, 20, 20 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			// Black inner rectangle for "2 PLAYERS" option
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			backgroundBlack = { 582, 202, 16, 16 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			// Text destination for "2 PLAYERS"
			textDstRect = { 610, 202, 70, 20 };
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
			SDL_Rect backgroundWhite = { 400, 245, 280, 70 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 405, 250, 270, 60 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 472, 262, 140, 40 };
			text->loadFromRenderedText(gRenderer, gFont, "CLASSIC", &textDstRect);
		}
		// Double Enemy
		{ 
			SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			if (flags->isTwoPlayer) SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 90);

			SDL_Rect backgroundWhite = { 400, 325, 280, 70 }; 
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 405, 330, 270, 60 }; 
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 422, 342, 240, 40 }; 
			text->loadFromRenderedText(gRenderer, gFont, "DOUBLE ENEMY", &textDstRect, flags->isTwoPlayer);
		}
		// Double Paddle
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 400, 405, 280, 70 }; 
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 405, 410, 270, 60 }; 
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 422, 422, 240, 40 }; 
			text->loadFromRenderedText(gRenderer, gFont, "DOUBLE PADDLE", &textDstRect);

		}
		// Quit
		{ 
			SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
			SDL_Rect backgroundWhite = { 400, 525, 280, 70 };
			SDL_RenderFillRect(gRenderer, &backgroundWhite);

			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_Rect backgroundBlack = { 405, 530, 270, 60 };
			SDL_RenderFillRect(gRenderer, &backgroundBlack);

			SDL_Rect textDstRect = { 494, 542, 90, 40 }; 
			text->loadFromRenderedText(gRenderer, gFont, "QUIT", &textDstRect);
		}
	}

	if (flags->inPlaying) {
		SDL_ShowCursor(SDL_DISABLE);

		static uint8_t scoreLeft = 0;
		static uint8_t scoreRight = 0;

		// Render paddles
		{
			// Render left and right paddles, and center line
			drawCenterLines();
			leftPaddle->render();
			rightPaddle->render();

			// Render additional center paddles if double gameplay is chosen
			if (flags->isDoubleEnemy || flags->isDoublePaddle) {
				centerLeftPaddle->render();
				centerRightPaddle->render();
			}
		}


		// Render pong
		{
			if (pong1->spawned)	pong1->render();

			if ((flags->isDoubleEnemy || flags->isDoublePaddle) && pong2->spawned) {
				pong2->render();
			}
		}
	}

	SDL_RenderPresent(gRenderer);
}

const bool& Game::isRunning() const {
	return running;
}