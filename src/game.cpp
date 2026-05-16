// here we have the actual logic to run our game

#include <cctype>
#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include "game.h"
#include "position.h"


// general method for generting random positions in the grid
Position Game::getRandomPosition() {

    // define distribution range
    std::uniform_int_distribution<> distr_height(0, this->height - 1); 
    std::uniform_int_distribution<> distr_width(0, this->width - 1); 
    
    int random_row = distr_height(gen);
    int random_col = distr_width(gen);

    return Position{random_row, random_col};
}


// our grid is fixed for now
Game::Game() 
    :width{40}, height{20}, move_direction{0, 0}, points{0}, running{true}
{
    // create the generator
    // initialize a seed source
    std::random_device rd;

    // initialize generator engine with seed
    std::mt19937 gen(rd());

    Position snake_head = getRandomPosition();
    snake.push_front(snake_head);

    do {
        food_position = getRandomPosition();
    } while (hits_snake(snake, food_position));

    // I'll let the player initialize the game grid
    // std::cout << "ENTER THE GAME WIDTH: \n"
    // std::cin >> width;
    // std::cout << "ENTER THE GAME HEIGHT: \n"
    // std::cin >> height;
}

void Game::handleInput() {
    char command;
    std::cin >> command;
    // normalize to lower case
    command = static_cast<char>(std::tolower(static_cast<unsigned char>(command)));
    switch(command) {
        case 'w':
            move_direction.row = -1;
            move_direction.col = 0;
            break;
        case 'a':
            move_direction.row = 0;
            move_direction.col = -1;
            break;
        case 's':
            move_direction.row = 1;
            move_direction.col = 0;
            break;
        case 'd':
            move_direction.row = 0;
            move_direction.col = 1;
            break;
        case 'q':
            running = false;
            break;
        // if the key isn't valid we just quit the game
        default:
            std::cout << "USE THE WASD PAD TO MOVE OR q TO QUIT THE GAME!\n";
    }
}

bool hits_snake(const std::deque<Position>& snake, const Position& pos) {
    return std::find(snake.begin(), snake.end(), pos) != snake.end();
}

void Game::update() {
    // get the new head of the snake and add it to the front
    Position new_head = snake.front();
    new_head.row += move_direction.row;
    new_head.col += move_direction.col;
    snake.push_front(new_head);


    // check if the snake head is still in bounds
    if (new_head.row < 0 || new_head.row >= height || 
        new_head.col < 0 || new_head.col >= width ) {
            running = false;
    }

    // see if we ate food; if so, keep the tail
    if (new_head.row == food_position.row && new_head.col == food_position.col) {
        points += 1;
        // randomize the food for next turn
        do {
            food_position = getRandomPosition();
        } while (hits_snake(snake, food_position));
    } else {
        // if we didn't eat food, we pop the tail
        snake.pop_back();
    }

    // zero out move_direction for next turn just in case we click an invalid char
    move_direction = Position{0, 0};


}

void Game::draw() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == snake.front().row && j == snake.front().col) {
                std::cout << "O";
            } else if (i == food_position.row && j == food_position.col) {
                std::cout << "*";
            } else if (hits_snake(snake, Position{i, j})) {
                std::cout << "o";
            } else {
                std::cout << ".";
            }
        }
        std::cout << '\n';
    }
    std::cout << "ENTER YOUR MOVE: ";
}

// game loop
void Game::run() {
    while (running) {
        draw();
        handleInput();
        update();
    }
    std::cout << "THE GAME HAS ENDED. YOU GOT " << points << " POINTS!\n";
}
