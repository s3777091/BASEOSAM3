#include "game.h"
#include "map.h"
#include "core.h"

// Variables
struct Snake snake;
struct Teleport teleport;

void initializeGame()
{

    initializeMap();
    wait_ms(5000);
    snake.length = 2;
    snake.direction = 1; // Initial direction: right

    // Place snake above the first brick
    for (int j = 0; j < snake.length; j++)
    {
        snake.body[j].x = map_one.bricks[0].x + snake.length;
        snake.body[j].y = map_one.bricks[0].y - 1;
    }

}

void drawSnake()
{
    for (int i = 0; i < snake.length; i++)
    {
        drawRect(snake.body[i].x * CELL_SIZE, snake.body[i].y * CELL_SIZE,
                 (snake.body[i].x * CELL_SIZE) + CELL_SIZE - 1,
                 (snake.body[i].y * CELL_SIZE) + CELL_SIZE - 1, 0x2, 1);
    }
}

void drawFood()
{
    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (map_one.apples[i].x >= 0 && map_one.apples[i].y >= 0)
        {
            drawRect(map_one.apples[i].x, map_one.apples[i].y, map_one.apples[i].x + CELL_SIZE - 1, map_one.apples[i].y + CELL_SIZE - 1, 0x4, 1);
        }
    }
}

void drawTeleport()
{
    drawRect(teleport.x, teleport.y, teleport.x + CELL_SIZE - 1, teleport.y + CELL_SIZE - 1, 0x3, 1);
}

void drawBricks()
{
    for (int i = 0; i < MAX_BRICKS; i++)
    {
        if (map_one.bricks[i].length > 0)
        {
            int x = map_one.bricks[i].x;
            int y = map_one.bricks[i].y;
            int length = map_one.bricks[i].length;

            if (map_one.bricks[i].direction == 0) // Horizontal
            {
                for (int j = 0; j < length; j++)
                {
                    drawRect((x + j) * CELL_SIZE, y * CELL_SIZE,
                             (x + j) * CELL_SIZE + CELL_SIZE - 1,
                             y * CELL_SIZE + CELL_SIZE - 1, 0x1, 1);
                }
            }
            else // Vertical
            {
                for (int j = 0; j < length; j++)
                {
                    drawRect(x * CELL_SIZE, (y + j) * CELL_SIZE,
                             x * CELL_SIZE + CELL_SIZE - 1,
                             (y + j) * CELL_SIZE + CELL_SIZE - 1, 0x1, 1);
                }
            }
        }
    }
}

void changeDirection(int newDirection)
{
    if (newDirection == 0 && snake.direction != 1)
        snake.direction = 0;
    if (newDirection == 1 && snake.direction != 0)
        snake.direction = 1;
    if (newDirection == 2 && snake.direction != 3)
        snake.direction = 2;
    if (newDirection == 3 && snake.direction != 2)
        snake.direction = 3;
}

void moveSnake()
{
    // Move the snake in the current direction
    for (int i = snake.length - 1; i > 0; i--)
    {
        snake.body[i] = snake.body[i - 1];
    }

    switch (snake.direction)
    {
        case 0: // Left
            snake.body[0].x--;
            break;
        case 1: // Right
            snake.body[0].x++;
            break;
        case 2: // Up
            snake.body[0].y--;
            break;
        case 3: // Down
            snake.body[0].y++;
            break;
    }
}

void applyGravity()
{
    if (!snake.onBrick)
    {
        uart_puts("Game Over\n");
    }
}

void checkCollision()
{
    // Check if snake eats the food
    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (snake.body[0].x == map_one.apples[i].x &&
            snake.body[0].y == map_one.apples[i].y)
        {
            if (snake.length < MAX_SNAKE_LENGTH)
            {
                snake.length++;
            }
            map_one.apples[i].x = -1; // Remove apple
            map_one.apples[i].y = -1; // Remove apple
        }
    }



}

void checkOnBrick()
{
    snake.onBrick = 0; 
    for (int i = 0; i < MAX_BRICKS; i++)
    {
        if (map_one.bricks[i].length > 0)
        {
            int x = map_one.bricks[i].x;
            int y = map_one.bricks[i].y;
            int length = map_one.bricks[i].length;

            // Horizontal brick
            if (map_one.bricks[i].direction == 0)
            {
                if (snake.body[0].y == y - 1)
                {
                    if ((snake.body[0].x + snake.length) - 1 < x || (x + length) < snake.body[0].x)
                    {
                        uart_puts("out brick\n");
                    }
                    else
                    {
                        uart_puts("on brick\n");
                        snake.onBrick = 1;
                        break;
                    }
                }
            }
            // Vertical brick
            else
            {
                
            }
        }
    }
}




void playGame()
{
    initializeGame();
    wait_ms(5000);

    while (1)
    {
        char c = uart_getc();
        switch (c)
        {
            case 'w': changeDirection(2); break;
            case 's': changeDirection(3); break;
            case 'a': changeDirection(0); break;
            case 'd': changeDirection(1); break;
        }

        moveSnake();
        checkCollision();
        checkOnBrick();

        clearScreen(0x0);

    
        drawBricks();
        drawSnake();
    }
}