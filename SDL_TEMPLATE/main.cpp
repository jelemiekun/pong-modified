#include "Game.h"
#include "FPSmanager.h"

int main(int argc, char* argv[]) {
    Game* game = new Game;

    game->init();

    Uint32 startTime = SDL_GetTicks();
    uint16_t countFrame = 0;

    while (game->isRunning()) {
        Uint32 frameStartTime = SDL_GetTicks();

        game->input();
        game->update();
        game->render();

        FPSmanager::limitFPS(frameStartTime);
        FPSmanager::calculateFPS(countFrame, startTime);
    }

    delete game;

    return 0;
}
