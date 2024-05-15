#include "framebf.h"
#include "uart.h"
#include "map.h"
#include "core.h"

// Definitions
#define MAX_SNAKE_LENGTH 10

#define SCR_WIDTH 1024
#define SCR_HEIGHT 768

#define BOUND_X (SCR_WIDTH / CELL_SIZE - 1)
#define BOUND_Y (SCR_HEIGHT / CELL_SIZE - 1)

#define LEFT  0
#define RIGHT 1
#define UP    2
#define DOWN  3

// Structures
struct SnakeSegment
{
    int x, y;
};

struct Snake
{
    struct SnakeSegment body[MAX_SNAKE_LENGTH];
    int length;
    int direction;
    int onBrick;    // Flag to check if the snake is on a brick
};

void playGame();
void drawFood();
void drawSnake();
void drawTeleport();
void drawBricks();
void initializeGame();
void moveSnake();
int getDirection(char dir);
int isValidMove(int newDirection);
void checkCollision();
void applyGravity();