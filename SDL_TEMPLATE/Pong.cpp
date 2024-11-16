#include "Pong.h"
#include <random>
#include <iostream>

Pong::Pong() : Object(), dirX(DIRECTIONS::NONE), dirY(DIRECTIONS::NONE),
                spawned(false) {}

Pong::~Pong() {}

uint16_t Pong::SCREEN_WIDTH = 0;
uint16_t Pong::SCREEN_HEIGHT = 0;
bool Pong::isClassic = 0;

void Pong::move(const bool& isUp, const int& velocity) {
    switch (dirX) {
    case Pong::DIRECTIONS::POSITIVE: dstRect->x += velocity; break;
    case Pong::DIRECTIONS::NEGATIVE: dstRect->x -= velocity; break;
    case Pong::DIRECTIONS::NONE: break;
    default: break;
    }

    switch (dirY) {
    case Pong::DIRECTIONS::POSITIVE: dstRect->y += velocity; break;
    case Pong::DIRECTIONS::NEGATIVE: dstRect->y -= velocity; break;
    case Pong::DIRECTIONS::NONE: break;
    default: break;
    }

    if ((dstRect->y < 0) || (dstRect->y + dstRect->h > SCREEN_HEIGHT)) {
        if (dirY == DIRECTIONS::POSITIVE) dirY = DIRECTIONS::NEGATIVE;
        else if (dirY == DIRECTIONS::NEGATIVE) dirY = DIRECTIONS::POSITIVE;
    }

    if (isClassic) {
        if ((dstRect->x + dstRect->w) < 0 || dstRect->x > SCREEN_WIDTH) {
            spawned = false;
            dirX = DIRECTIONS::NONE;
            dirY = DIRECTIONS::NONE;
        }
    } else {
        if ((dstRect->x + dstRect->w) < 0 || dstRect->x > SCREEN_WIDTH ||
            (dstRect->x + dstRect->w) < SCREEN_WIDTH / 2 || dstRect->x > SCREEN_WIDTH / 2) {
            spawned = false;
            dirX = DIRECTIONS::NONE;
            dirY = DIRECTIONS::NONE;
        }
    }

}

void Pong::spawn(const bool& isLeft) {
    static std::random_device dev1;
    static std::mt19937 rng1(dev1());
    std::uniform_int_distribution<std::mt19937::result_type> distXX(0, 1);
    int generatedRandomXDirection = distXX(rng1);

    switch (generatedRandomXDirection) {
    case 0: dirX = DIRECTIONS::POSITIVE; break;
    case 1: dirX = DIRECTIONS::NEGATIVE; break;
    default: dirX = DIRECTIONS::POSITIVE; break;
    }

    static std::random_device dev2;
    static std::mt19937 rng2(dev2());
    std::uniform_int_distribution<std::mt19937::result_type> distYY(0, 2);

    int generatedRandomYDirection = distYY(rng2);
    switch (generatedRandomYDirection) {
    case 0: dirY = DIRECTIONS::POSITIVE; break;
    case 1: dirY = DIRECTIONS::NEGATIVE; break;
    case 2: dirY = DIRECTIONS::NONE; break;
    default: dirY = DIRECTIONS::POSITIVE; break;
    }

    static std::random_device dev3;
    static std::mt19937 rng3(dev3());
    std::uniform_int_distribution<std::mt19937::result_type> distYPos(0, (SCREEN_HEIGHT - dstRect->h));

    if (isClassic) {
        dstRect->x = SCREEN_WIDTH / 2;
    } else {
        std::cout << "meow" << '\n';
        if (isLeft) dstRect->x = SCREEN_WIDTH / 4;
        else dstRect->x = (SCREEN_WIDTH / 4) * 3;
    }


    dstRect->y = distYPos(rng3);

    spawned = true;
}