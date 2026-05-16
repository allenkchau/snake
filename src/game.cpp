// here we have the actual logic to run our game

#include <cctype>
#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include "game.h"
#include "position.h"

// helper methods for game logics

bool hitsSnake(const std::deque<Position>& snake, const Position& pos) {
    return std::find(snake.begin(), snake.end(), pos) != snake.end();
}

bool Game::isOutOfBounds(const Position& pos) const {
    return pos.row < 0 || pos.row >= height || pos.col < 0 || pos.col >= width;
}

bool Game::isSelfCollision(const Position& pos, bool will_grow) const {
    auto end = snake.end();
    // If we are not growing, the tail will move away,
    // so moving into the current tail position is allowed.
    if (!will_grow && !snake.empty()) {
        --end;
    }
    return std::find(snake.begin(), end, pos) != end;
}

void Game::placeFood() {
    do {
        food_position = getRandomPosition();
    } while (hitsSnake(snake, food_position));
}

bool Game::isEatingFood(const Position& pos) const {

    return pos.row == food_position.row && pos.col == food_position.col;

}

// general method for generting random positions in the grid
Position Game::getRandomPosition() {

    // define distribution range
    std::uniform_int_distribution<> distr_height(0, this->height - 1); 
    std::uniform_int_distribution<> distr_width(0, this->width - 1); 
    
    int random_row = distr_height(generator);
    int random_col = distr_width(generator);

    return Position{random_row, random_col};
}


// our grid is fixed for now
Game::Game() 
    :width{40}, height{20}, move_direction{0, 0}, points{0}, running{true}, generator{std::random_device{}()}
{
    // // create the generator
    // // initialize a seed source
    // std::random_device rd;

    // // initialize generator engine with seed
    // generator = std::mt19937 gen(rd());

    Position snake_head = getRandomPosition();
    snake.push_front(snake_head);

    placeFood();

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


void Game::update() {
    // Compute candidate next head position.
    Position new_head = snake.front();
    new_head.row += move_direction.row;
    new_head.col += move_direction.col;

    // Wall collision.
    if (isOutOfBounds(new_head)) {
        running = false;
        return;
    }

    bool will_grow = isEatingFood(new_head);

    // Body collision.
    // If not growing, moving into the old tail cell is allowed because
    // the tail moves away on this turn.
    if (isSelfCollision(new_head, will_grow)) {
        running = false;
        return;
    }

    // Move snake.
    snake.push_front(new_head);

    if (will_grow) {
        points += 1;

        if (static_cast<int>(snake.size()) == height * width) {
            running = false;
            std::cout << "YOU WON THE GAME!!\n";
            return;
        }

        placeFood();
    } else {
        snake.pop_back();
    }
}

void Game::draw() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == snake.front().row && j == snake.front().col) {
                std::cout << "O";
            } else if (i == food_position.row && j == food_position.col) {
                std::cout << "*";
            } else if (hitsSnake(snake, Position{i, j})) {
                std::cout << "o";
            } else {
                std::cout << ".";
            }
        }
        std::cout << '\n';
    }
    std::cout << "\nENTER YOUR MOVE: \n";
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
