#ifndef GAME_H
#define GAME_H

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

extern struct Snake snake; // Declare snake as an external variable

void playGame();
void initializeGame();
void moveSnake();
int getDirection(char dir);
int isValidMove(int newDirection);
void checkCollision();
void applyGravity();
void drawRotatedImage(unsigned int image[32*32], int x, int y, int width, int height, int direction)

#endif // GAME_H