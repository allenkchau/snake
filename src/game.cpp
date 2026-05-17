// here we have the actual logic to run our game

#include <cctype>
#include <iostream>
#include <random>
#include <algorithm>
#include <deque>
#include <ncurses.h>
#include "game.h"
#include "position.h"

// helper methods for game logics

bool hitsSnake(const std::deque<Position>& snake, const Position& pos) {
    return std::find(snake.begin(), snake.end(), pos) != snake.end();
}

bool Game::isOutOfBounds(const Position& pos) const {
    return pos.row < 1 || pos.row >= height - 1 || pos.col < 1 || pos.col >= width - 1;
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
    std::uniform_int_distribution<> distr_height(1, this->height - 2); 
    std::uniform_int_distribution<> distr_width(1, this->width - 2); 
    
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
    int command = getch();
    if (command == ERR) {
        return;
    }

    switch (command) {
        case 'w':
        case 'W':
        case KEY_UP:
            move_direction.row = -1;
            move_direction.col = 0;
            break;
        case 'a':
        case 'A':
        case KEY_LEFT:
            move_direction.row = 0;
            move_direction.col = -1;
            break;
        case 's':
        case 'S':
        case KEY_DOWN:
            move_direction.row = 1;
            move_direction.col = 0;
            break;
        case 'd':
        case 'D':
        case KEY_RIGHT:
            move_direction.row = 0;
            move_direction.col = 1;
            break;
        case 'q':
        case 'Q':
            running = false;
            break;
        default:
            break;
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
            return;
        }
        // randomly place food for the next turn
        placeFood();
    } else {
        snake.pop_back();
    }
}

void Game::draw() {
    clear();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // draw the barrier
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) {
                mvaddch(i, j, '#');
            }
            else if (i == snake.front().row && j == snake.front().col) {
                mvaddch(i, j, 'O');
            } else if (i == food_position.row && j == food_position.col) {
                mvaddch(i, j, '*');
            } else if (hitsSnake(snake, Position{i, j})) {
                mvaddch(i, j, 'o');
            } else {
                mvaddch(i, j, ' ');
            }
        }
    }
    mvprintw(height, 0, "Score: %d  Controls: WASD/Arrows, Q to quit", points);
    refresh();
}

// game loop
void Game::run() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);
    nodelay(stdscr, true);
    curs_set(0);

    while (running) {
        handleInput();
        update();
        draw();
        napms(120);
    }

    endwin();
    std::cout << "THE GAME HAS ENDED. YOU GOT " << points << " POINTS!\n";
}
