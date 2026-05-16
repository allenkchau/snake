// interface for our game

// include guards (prevent multiple inclusions)
// we can't have multiple inclusions of header files because of the ODR
// ODR - a type/class/function definition should only appear once per translation unit; also global vars and noninline funcs should not be repeated (linker errors)
// a translation unit is a .cpp file and all its included headers after preprocessing
// we can have multiple declarations in a translation unit but there must only be a single definition

// #pragma once does exactly the same thing as the guards; a pragma is just a directive that tells the compiler how it should process the code

#ifndef GAME_H
#define GAME_H

#include <deque>
#include <random>
#include "position.h"


class Game {
    private:
        // dimensions of our game grid
        const int width;
        const int height;
        // sequence of positions representing the snake
        std::deque<Position> snake;
        // direction we move in based on pad key
        Position move_direction;
        int points;

        Position food_position;

        // whether game has ended or not
        bool running;

        // random number generator
        std::mt19937 generator;

    public:
        // constructor
        Game();

        // member functions
        // start the game in its loop
        void run();
        // read a move (WASD key pad)
        void handleInput();
        // update game state
        void update();
        // render new game in terminal
        void draw();
        // randomly get a position on the grid
        Position getRandomPosition();

        // check if we are out of bounds
        bool isOutOfBounds(const Position& pos) const;

        bool isSelfCollision(const Position& pos, bool will_grow) const;

        void placeFood();

        bool isEatingFood(const Position& pos) const;
};


#endif // GAME_H
