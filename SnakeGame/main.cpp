// Snake Game - Complete working version
#define _WIN32_WINNT 0x0A00  // Windows 10

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

// Game constants
const int WIDTH = 60;
const int HEIGHT = 20;

// Game structures
struct Position {
    int x, y;
};

struct Snake {
    Position head;
    Position tail[100];
    int length;
    int direction;  // 1=up, 2=down, 3=left, 4=right
    int score;
    bool gameOver;
};

// Global variables
Snake snake;
Position food;
string playerName;

// Function prototypes
void Setup();
void Draw();
void Input();
void Logic();
void GenerateFood();
int GetGameSpeed();

// Main game setup
void Setup() {
    snake.gameOver = false;
    snake.direction = 4;  // Start moving right
    snake.head.x = WIDTH / 2;
    snake.head.y = HEIGHT / 2;
    snake.length = 0;
    snake.score = 0;

    srand(static_cast<unsigned>(time(nullptr)));
    GenerateFood();
}

// Draw game screen
void Draw() {
    system("cls");  // Clear screen

    // Draw top border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    // Draw game area
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Draw left border
            if (x == 0) cout << "#";

            // Draw snake head
            if (x == snake.head.x && y == snake.head.y)
                cout << "O";
            // Draw food
            else if (x == food.x && y == food.y)
                cout << "F";
            // Draw snake tail
            else {
                bool tailDrawn = false;
                for (int i = 0; i < snake.length; i++) {
                    if (snake.tail[i].x == x && snake.tail[i].y == y) {
                        cout << "o";
                        tailDrawn = true;
                        break;
                    }
                }
                if (!tailDrawn) cout << " ";
            }

            // Draw right border
            if (x == WIDTH - 1) cout << "#";
        }
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    // Display game info
    cout << "\nPlayer: " << playerName << endl;
    cout << "Score: " << snake.score << endl;
    cout << "Length: " << snake.length << endl;
    cout << "Controls: W=Up, A=Left, S=Down, D=Right, Q=Quit" << endl;
}

// Handle keyboard input
void Input() {
    if (_kbhit()) {
        switch (_getch()) {
        case 'w':
        case 'W':
            if (snake.direction != 2) snake.direction = 1;
            break;
        case 's':
        case 'S':
            if (snake.direction != 1) snake.direction = 2;
            break;
        case 'a':
        case 'A':
            if (snake.direction != 4) snake.direction = 3;
            break;
        case 'd':
        case 'D':
            if (snake.direction != 3) snake.direction = 4;
            break;
        case 'q':
        case 'Q':
            snake.gameOver = true;
            break;
        }
    }
}

// Game logic
void Logic() {
    // Save previous head position for tail movement
    Position prevHead = snake.head;
    Position prevTail;

    // Move tail
    if (snake.length > 0) {
        prevTail = snake.tail[0];
        snake.tail[0] = prevHead;

        for (int i = 1; i < snake.length; i++) {
            Position temp = snake.tail[i];
            snake.tail[i] = prevTail;
            prevTail = temp;
        }
    }

    // Move head based on direction
    switch (snake.direction) {
    case 1: snake.head.y--; break; // Up
    case 2: snake.head.y++; break; // Down
    case 3: snake.head.x--; break; // Left
    case 4: snake.head.x++; break; // Right
    }

    // Check wall collision
    if (snake.head.x < 0 || snake.head.x >= WIDTH ||
        snake.head.y < 0 || snake.head.y >= HEIGHT) {
        snake.gameOver = true;
    }

    // Check self collision
    for (int i = 0; i < snake.length; i++) {
        if (snake.head.x == snake.tail[i].x && snake.head.y == snake.tail[i].y) {
            snake.gameOver = true;
        }
    }

    // Check food collision
    if (snake.head.x == food.x && snake.head.y == food.y) {
        snake.score += 10;
        snake.length++;
        GenerateFood();
    }
}

// Generate new food position
void GenerateFood() {
    bool onSnake;

    do {
        onSnake = false;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;

        // Check if food spawns on snake head
        if (food.x == snake.head.x && food.y == snake.head.y)
            onSnake = true;

        // Check if food spawns on snake tail
        for (int i = 0; i < snake.length; i++) {
            if (food.x == snake.tail[i].x && food.y == snake.tail[i].y) {
                onSnake = true;
                break;
            }
        }
    } while (onSnake);
}

// Get game speed based on difficulty
int GetGameSpeed() {
    int choice;
    cout << "\n=== SNAKE GAME ===\n";
    cout << "Select difficulty:\n";
    cout << "1. Easy (Slow)\n";
    cout << "2. Medium (Normal)\n";
    cout << "3. Hard (Fast)\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
    case 1: return 150;  // Easy
    case 3: return 50;   // Hard
    default: return 100; // Medium
    }
}

// Main function
int main() {
    // Get player name
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, playerName);

    if (playerName.empty()) {
        playerName = "Player";
    }

    // Get game speed
    int speed = GetGameSpeed();

    // Setup game
    Setup();

    // Main game loop
    while (!snake.gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(speed);
    }

    // Game over screen
    system("cls");
    cout << "\n\n";
    cout << "  ###############################\n";
    cout << "  #       GAME OVER!           #\n";
    cout << "  ###############################\n\n";
    cout << "  Player: " << playerName << endl;
    cout << "  Final Score: " << snake.score << endl;
    cout << "  Snake Length: " << snake.length << endl;
    cout << "\n  Press any key to exit...";

    _getch();
    return 0;
}