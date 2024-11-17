#include "Pong.h"
#include <random>
#include <iostream>

Pong::Pong() : Object(), dirX(DIRECTIONS::NONE), dirY(DIRECTIONS::NONE),
            spawned(false), scored(false), playerScored(false) {}

Pong::~Pong() {}

bool Pong::isClassic = 0;

void Pong::scaleColliders(const double& scale) {
    for (int i = 0; i < std::size(colliders); i++) {
        colliders[i].w *= scale;
        colliders[i].h *= scale;
    }
}

void Pong::move(const bool& isLeft, const int& velocity, Paddle*& paddleLeft, Paddle*& paddleRight) {
    int actualVelocityX = 0;
    int actualVelocityY = 0;

    // Move the pong
    {
        switch (dirX) {
        case Pong::DIRECTIONS::POSITIVE:
            actualVelocityX = velocity;
            dstRect->x += actualVelocityX;
            break;
        case Pong::DIRECTIONS::NEGATIVE:
            actualVelocityX = -velocity;
            dstRect->x += actualVelocityX;
            break;
        case Pong::DIRECTIONS::NONE:
            actualVelocityX = 0;
            break;
        default:
            break;
        }

        switch (dirY) {
        case Pong::DIRECTIONS::POSITIVE:
            actualVelocityY = velocity;
            dstRect->y += actualVelocityY;
            break;
        case Pong::DIRECTIONS::NEGATIVE:
            actualVelocityY = -velocity;
            dstRect->y += actualVelocityY;
            break;
        case Pong::DIRECTIONS::NONE:
            actualVelocityY = 0;
            break;
        default:
            break;
        }
    }

    // Check if goes beyond the line
    {
        if ((dstRect->y < 0) || (dstRect->y + dstRect->h > SCREEN_HEIGHT)) {
            if (dirY == DIRECTIONS::POSITIVE) dirY = DIRECTIONS::NEGATIVE;
            else if (dirY == DIRECTIONS::NEGATIVE) dirY = DIRECTIONS::POSITIVE;
        }

        if (isClassic) {
            if ((dstRect->x + dstRect->w) < 0 || dstRect->x > SCREEN_WIDTH) {
                playerScored = dstRect->x > SCREEN_WIDTH;

                scored = true;
                spawned = false;
                dirX = DIRECTIONS::NONE;
                dirY = DIRECTIONS::NONE;
            }
        } else {
            if (isLeft) {
                if ((dstRect->x + dstRect->w) < 0 || (dstRect->x > SCREEN_WIDTH / 2)) {
                    playerScored = (dstRect->x + dstRect->w) < 0;

                    scored = true;
                    spawned = false;
                    dirX = DIRECTIONS::NONE;
                    dirY = DIRECTIONS::NONE;
                }
            } else {
                if ((dstRect->x + dstRect->w) < SCREEN_WIDTH / 2 || (dstRect->x > SCREEN_WIDTH)) {
                    playerScored = (dstRect->x > SCREEN_WIDTH);

                    scored = true;
                    spawned = false;
                    dirX = DIRECTIONS::NONE;
                    dirY = DIRECTIONS::NONE;
                }
            }
        }
    }

    // Check collisions
    {
        bool collided = false;
        bool invertX = false;
        bool invertY = false;

        // Function to handle direction inversion based on collision
        auto handleCollision = [&](const std::string& side) {
            if (side == "left" || side == "right") {
                if (dirX == DIRECTIONS::POSITIVE && dirY == DIRECTIONS::NONE) {
                    invertX = true;
                    dirY = DIRECTIONS::POSITIVE; // Make Y positive
                } else if (dirX == DIRECTIONS::NEGATIVE && dirY == DIRECTIONS::NONE) {
                    invertX = true;
                    dirY = DIRECTIONS::POSITIVE; // Make Y positive
                } else {
                    // Handle normal cases
                    if (side == "left") {
                        if (dirX == DIRECTIONS::POSITIVE && dirY == DIRECTIONS::POSITIVE) invertX = true;
                        else if (dirX == DIRECTIONS::POSITIVE && dirY == DIRECTIONS::NEGATIVE) invertX = true;
                    } else if (side == "right") {
                        if (dirX == DIRECTIONS::NEGATIVE && dirY == DIRECTIONS::POSITIVE) invertX = true;
                        else if (dirX == DIRECTIONS::NEGATIVE && dirY == DIRECTIONS::NEGATIVE) invertX = true;
                    }
                }
            } else if (side == "top") {
                if (dirY == DIRECTIONS::POSITIVE && dirX == DIRECTIONS::POSITIVE) invertY = true;
                else if (dirY == DIRECTIONS::POSITIVE && dirX == DIRECTIONS::NEGATIVE) invertY = true;
            } else if (side == "bottom") {
                if (dirY == DIRECTIONS::NEGATIVE && dirX == DIRECTIONS::POSITIVE) invertY = true;
                else if (dirY == DIRECTIONS::NEGATIVE && dirX == DIRECTIONS::NEGATIVE) invertY = true;
            }
            };

        // Check collision with the left paddle
        for (int i = 0; i < std::size(paddleLeft->colliders); i++) {
            for (int j = 0; j < std::size(colliders); j++) {
                if (!collided) {
                    int leftA = colliders[j].x;
                    int rightA = colliders[j].x + colliders[j].w;
                    int topA = colliders[j].y;
                    int bottomA = colliders[j].y + colliders[j].h;

                    int leftB = paddleLeft->colliders[i].x;
                    int rightB = paddleLeft->colliders[i].x + paddleLeft->colliders[i].w;
                    int topB = paddleLeft->colliders[i].y;
                    int bottomB = paddleLeft->colliders[i].y + paddleLeft->colliders[i].h;

                    if (bottomA < topB) continue;
                    if (topA > bottomB) continue;
                    if (rightA < leftB) continue;
                    if (leftA > rightB) continue;

                    collided = true;

                    // Determine which side of the paddle was hit
                    if (rightA > leftB && leftA < leftB) {
                        handleCollision("left");
                    } else if (leftA < rightB && rightA > rightB) {
                        handleCollision("right");
                    } else if (bottomA > topB && topA < topB) {
                        handleCollision("top");
                    } else if (topA < bottomB && bottomA > bottomB) {
                        handleCollision("bottom");
                    }
                }
            }
        }

        // Check collision with the right paddle
        if (!collided) {
            for (int i = 0; i < std::size(paddleRight->colliders); i++) {
                for (int j = 0; j < std::size(colliders); j++) {
                    if (!collided) {
                        int leftA = colliders[j].x;
                        int rightA = colliders[j].x + colliders[j].w;
                        int topA = colliders[j].y;
                        int bottomA = colliders[j].y + colliders[j].h;

                        int leftB = paddleRight->colliders[i].x;
                        int rightB = paddleRight->colliders[i].x + paddleRight->colliders[i].w;
                        int topB = paddleRight->colliders[i].y;
                        int bottomB = paddleRight->colliders[i].y + paddleRight->colliders[i].h;

                        if (bottomA < topB) continue;
                        if (topA > bottomB) continue;
                        if (rightA < leftB) continue;
                        if (leftA > rightB) continue;

                        collided = true;

                        // Determine which side of the paddle was hit
                        if (rightA > leftB && leftA < leftB) {
                            handleCollision("left");
                        } else if (leftA < rightB && rightA > rightB) {
                            handleCollision("right");
                        } else if (bottomA > topB && topA < topB) {
                            handleCollision("top");
                        } else if (topA < bottomB && bottomA > bottomB) {
                            handleCollision("bottom");
                        }
                    }
                }
            }
        }

        // Apply the direction changes based on collision
        if (invertX) dirX = (dirX == DIRECTIONS::POSITIVE) ? DIRECTIONS::NEGATIVE : DIRECTIONS::POSITIVE;
        if (invertY) dirY = (dirY == DIRECTIONS::POSITIVE) ? DIRECTIONS::NEGATIVE : DIRECTIONS::POSITIVE;
    }



    // Update colliders
    moveColliders(dirY == DIRECTIONS::NEGATIVE, actualVelocityX, actualVelocityY);
}


void Pong::moveColliders(const bool& isUp, const int& velocityX, const int& velocityY) {
    for (int i = 0; i < std::size(colliders); i++) {
        colliders[i].x += velocityX;
        colliders[i].y += velocityY;
    }
}


void Pong::spawn(const bool& isLeft, const int& SCALE) {
    std::random_device dev1;
    std::mt19937 rng1(dev1());
    std::uniform_int_distribution<std::mt19937::result_type> distXX(0, 1);
    int generatedRandomXDirection = distXX(rng1);

    switch (generatedRandomXDirection) {
    case 0: dirX = DIRECTIONS::POSITIVE; break;
    case 1: dirX = DIRECTIONS::NEGATIVE; break;
    default: dirX = DIRECTIONS::POSITIVE; break;
    }

    std::random_device dev2;
    std::mt19937 rng2(dev2());
    std::uniform_int_distribution<std::mt19937::result_type> distYY(0, 2);

    int generatedRandomYDirection = distYY(rng2);
    switch (generatedRandomYDirection) {
    case 0: dirY = DIRECTIONS::POSITIVE; break;
    case 1: dirY = DIRECTIONS::NEGATIVE; break;
    case 2: dirY = DIRECTIONS::NONE; break;
    default: dirY = DIRECTIONS::POSITIVE; break;
    }

    std::random_device dev3;
    std::mt19937 rng3(dev3());
    std::uniform_int_distribution<std::mt19937::result_type> distYPos(0, (SCREEN_HEIGHT - dstRect->h));

    if (isClassic) {
        dstRect->x = SCREEN_WIDTH / 2;
    } else {
        if (isLeft) dstRect->x = SCREEN_WIDTH / 4;
        else dstRect->x = (SCREEN_WIDTH / 4) * 3;
    }

    dstRect->y = distYPos(rng3);

    {
        colliders[0].x = dstRect->x + (SCALE * 1);
        colliders[0].y = dstRect->y;

        colliders[1].x = dstRect->x;
        colliders[1].y = dstRect->y + (SCALE * 1);
    }

    spawned = true;
}