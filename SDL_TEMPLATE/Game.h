#pragma once
#include <memory>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"

#include "GameFlags.h"
#include "Text.h"
#include "Controller.h"
#include "TextureManager.h"
#include "Paddle.h"
#include "Pong.h"
#include "Timer.h"
#include "SFX.h"

class Game {
private:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Event gEvent;
	SDL_Cursor* gCursor;
	
	Controller* gGameController1;
	Controller* gGameController2;
	Controller* currentController;

	Paddle* leftPaddle;
	Paddle* centerLeftPaddle;
	Paddle* centerRightPaddle;
	Paddle* rightPaddle;

	uint8_t* player1Score;
	uint8_t* player2Score;
	uint8_t* bot1Score;
	uint8_t* bot2Score;

	Pong* pong1;
	Pong* pong2;

	Timer* timerPong1;
	Timer* timerPong2;

	GameFlags* flags;
	
	TTF_Font* gFont;

	SFX* sfx;
	
	Text* text;
	bool acceptKeyboardInput;
	bool running;
	bool paused;


private:
	constexpr static uint8_t CLASSIC_SCALE = 5;
	constexpr static uint8_t NOT_CLASSIC_SCALE = 2;
	constexpr static uint16_t SCREEN_WIDTH = 1080;
	constexpr static uint16_t SCREEN_HEIGHT = 720;
	constexpr static uint8_t CURSOR_SPEED = 5;
	constexpr static uint8_t OBJECTSPEED = 8;
	constexpr static uint8_t PONG_SPEED = 3;
	constexpr static Uint32 TIMER_DURATION = 500;

private:
	void initCursor();
	void initPaddlesAndCircle();
	void resetFlags();
	void drawCenterLines();
	void initClassicGame();
	void initDoubleEnemyOrPaddleGame();
	void freeGameResources();



public:
	Game();
	~Game();

	void init();
	void input();
	void update();
	void render();

	const bool& isRunning() const;
};

