#pragma once
#include <memory>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "GameFlags.h"
#include "Text.h"
#include "Controller.h"

class Game {
private:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Event gEvent;
	Controller* gGameController1;
	Controller* gGameController2;
	Controller* currentController;
	SDL_Cursor* gCursor;
	GameFlags* flags;
	TTF_Font* gFont;
	Text* text;
	bool acceptKeyboardInput;
	bool running;

private:
	constexpr static int SCREEN_WIDTH = 1080;
	constexpr static int SCREEN_HEIGHT = 720;

private:
	void initCursor();
	void resetFlags();

public:
	Game();
	~Game();

	void init();
	void input();
	void update();
	void render();

	const bool& isRunning() const;
};

