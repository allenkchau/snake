// here we have the actual logic to run our game

#include <iostream>
#include "game.h"
#include "position.h"


// our grid is always 20x20 for now
Game::Game() 
    :width{20}, height{20}, points{0}, running{false}
{}

Game Game::handleInput() {
    char command;
    std::cin >> command;
    switch(command) {
        case 'w':
            this->move_direction->row = -1
            this->move_direction->col = 0
            break;
        case 'a':
            this->move_direction->row = 0
            this->move_direction->col = -1
            break;
        case 's':
            this->move_direction->row = 0
            this->move_direction->col = 1
            break;
        case 'd':
            this->move_direction->row = 0
            this->move_direction->col = 1
            break;
        case 'q':
            this->running = false;
            break;
        // if the key isn't valid we just quit the game
        default:
            this->running = false;
    }
}

void Game::update() {
    this->player_position.row += this->move_direction.row;
    this->player_position.col += this->move_direction.col;
}

void Game::draw() {
    
}

// game loop
void Game::run() {
    while (this->running && ) {
        handleInput()
        update()
        draw()
    }
    cout << "THE GAME HAS ENDED. YOU GOT " << this->points << " POINTS!\n";
}
