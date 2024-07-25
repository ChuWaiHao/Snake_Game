#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <deque>
#include <string>
#include <fstream>

using namespace std;

const int W = 60; // Width of the grid
const int H = 30; // Height of the grid
const int U = 72; // Up arrow key
const int D = 80; // Down arrow key
const int L = 75; // Left arrow key
const int R = 77; // Right arrow key
const int ARROW = 224; // Arrow key identifier
const int ESC = 27; // Escape key

struct Point {
    int x, y;
    int type; // 0: normal, 1: special
};

deque<Point> snake; // deque to store snake body
Point food;         // food position

string playerName;  // player's name
int direction = R;  // initial direction to the right
int score = 0;      // score for the current game

vector<pair<string, int>> topPlayers; // vector to store top players

void generateFood() {
    srand(time(0));
    food.x = rand() % W;
    food.y = rand() % H;
    food.type = rand() % 2;
}

void initialize() {
    system("cls");  // clear console screen, start from empty
    cout << "Enter player name: ";
    cin >> playerName;

    // Clear any existing snake segments
    snake.clear();

    // Initialize the snake with a starting position and length
    Point start = { W / 2, H / 2 };
    //snake.push_back(start);
    snake.push_back({ start.x - 1, start.y }); // Initial length of the snake

    generateFood(); // Generate initial food position
}


void display() {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
            if (r == snake.front().y && c == snake.front().x)
                cout << "O"; // head of the snake
            else if (any_of(snake.begin() + 1, snake.end(), [&](Point p) { return p.x == c && p.y == r; }))
                cout << "o"; // body of the snake
            else if (r == food.y && c == food.x)
                cout << "*"; // food
            else
                cout << " ";
        }
        cout << endl;
    }

    cout << "Score: " << score << endl;
}

bool collision(Point newHead) {
    if (newHead.x < 0 || newHead.x >= W || newHead.y < 0 || newHead.y >= H) {
        return true; // collision with walls
    }
    if (any_of(snake.begin(), snake.end(), [&](Point p) { return p.x == newHead.x && p.y == newHead.y; })) {
        return true; // collision with itself
    }
    return false;
}
void resetGame() {
    // Store player's score in the leaderboard
    topPlayers.push_back({ playerName, score });

    // Ask player if they want to play again
    cout << "Do you want to play again? (Y/N): ";
    char playAgain;
    cin >> playAgain;

    if (playAgain == 'Y' || playAgain == 'y') {
        // Reset game variables
        snake.clear();
        score = 0; // Reset score
        initialize();
    }
    else {
        // Display leaderboard
        cout << "Top Players:\n";
        for (const auto& player : topPlayers) {
            cout << player.first << ": " << player.second << " points\n";
        }
        exit(0);
    }
}


void moveSnake() {
    Point newHead = snake.front();

    switch (direction) {
    case U: newHead.y--; break;
    case D: newHead.y++; break;
    case L: newHead.x--; break;
    case R: newHead.x++; break;
    }

    // Check if the new head position is out of bounds (hit the wall)
    if (newHead.x < 0 || newHead.x >= W || newHead.y < 0 || newHead.y >= H) {
        cout << "Game Over! You hit the wall." << endl;
        // Handle game over logic here
        resetGame();
        return;
    }

    // Check if the new head position collides with the snake's own body (hit itself)
    if (any_of(snake.begin() + 1, snake.end(), [&](Point p) { return p.x == newHead.x && p.y == newHead.y; })) {
        cout << "Game Over! You hit yourself." << endl;
        // Handle game over logic here
        resetGame();
        return;
    }

    snake.push_front(newHead);

    // Check if the new head position overlaps with the food
    if (newHead.x == food.x && newHead.y == food.y) {
        score += (food.type == 0 ? 10 : 10); // Increase score based on food type
        generateFood(); // Generate new food position
    }
    else {
        snake.pop_back(); // Remove the tail segment
    }

    // Display the game board with updated snake and food positions
    display();
}


int main() {
    initialize();

    while (true) {
        if (_kbhit()) {
            unsigned char c = _getch(); // get first key
            if (c == ARROW) {
                c = _getch(); // get second key
                switch (c) {
                case U: if (direction != D) direction = U; break; // up
                case L: if (direction != R) direction = L; break; // left
                case R: if (direction != L) direction = R; break; // right
                case D: if (direction != U) direction = D; break; // down
                }
            }
            else if (c == ESC) { // esc key
                moveSnake();
                display();
                return 0;
            }
        }

        moveSnake();
        display();
        Sleep(20); // Speed control
    }

    return 0;
}