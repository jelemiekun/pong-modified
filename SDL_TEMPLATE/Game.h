#pragma once
#include "SDL.h"
#include <memory>

class Game {
private:
	SDL_Window* gWindow;
	SDL_Renderer* gRenderer;
	SDL_Event gEvent;
	SDL_GameController* gGameController1;
	SDL_GameController* gGameController2;
	SDL_Cursor* gCursor;
	bool acceptKeyboardInput;
	bool running;

private:
	constexpr static int SCREEN_WIDTH = 1080;
	constexpr static int SCREEN_HEIGHT = 720;

private:
	void initCursor();

public:
	Game();
	~Game();

	void init();
	void input();
	void update();
	void render();

	const bool& isRunning() const;
};

