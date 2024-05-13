#include "framebf.h"
#include "uart.h"
#include "map.h"

// Definitions
#define MAX_SNAKE_LENGTH 100

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

#define BOUND_X (SCR_WIDTH / CELL_SIZE - 1)
#define BOUND_Y (SCR_HEIGHT / CELL_SIZE - 1)

// Structures
struct SnakeSegment
{
    int x, y;
};

struct Snake
{
    struct SnakeSegment body[MAX_SNAKE_LENGTH];
    int length;
    int direction;  // 0: left, 1: right, 2: up, 3: down
    int onBrick;    // Flag to check if the snake is on a brick
};

void playGame();
void drawFood();
void drawSnake();
void drawTeleport();
void drawBricks();
void initializeGame();
void moveSnake();
void changeDirection(int newDirection);
void checkCollision();
void applyGravity();