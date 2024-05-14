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
    snake.length = 3;
    snake.direction = LEFT; // Initial direction: right

    // Place snake above the first brick
    for (int i = 0; i < snake.length; i++)
    {
        snake.body[i].x = 16 + i;
        snake.body[i].y = 12;
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
        // Convert to Circle for Food
        drawRect(map_one.apples[i].x, map_one.apples[i].y, map_one.apples[i].x + CELL_SIZE, map_one.apples[i].y + CELL_SIZE, 0x4, 1);
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
        struct Brick temp = map_one.bricks[i];
        drawRect(temp.x * CELL_SIZE, temp.y * CELL_SIZE,
                (temp.x + temp.w) * CELL_SIZE,
                (temp.y + temp.h) * CELL_SIZE, 0x1, 1);
    }
}

int isValidMove(int move)
{
    if (move == LEFT && snake.direction != RIGHT) {
        snake.direction = LEFT;
        for (int i = 0; i < MAX_BRICKS; i++)
        {
            if (snake.body[0].x == map_one.bricks[i].x + 1)
            {
                return 0;
            }
        }
        return 1;
    }
        
    if (move == RIGHT && snake.direction != LEFT) {
        snake.direction = RIGHT;
        for (int i = 0; i < MAX_BRICKS; i++)
        {
            if (snake.body[0].x == map_one.bricks[i].x - 1)
            {
                return 0;
            }
        }
        return 1;
    }
        
    if (move == UP && snake.direction != DOWN) {
        snake.direction = UP;
        for (int i = 0; i < MAX_BRICKS; i++)
        {
            if (snake.body[0].y == map_one.bricks[i].y + 1)
            {
                return 0;
            }
        }
        return 1;
    }
        
    if (move == DOWN && snake.direction != UP) {
        snake.direction = DOWN;
        for (int i = 0; i < MAX_BRICKS; i++)
        {
            if (snake.body[0].y == map_one.bricks[i].y - 1)
            {
                return 0;
            }
        }
        return 1;
    }
        
    return 0;
}

int getDirection(char dir) {
    switch (dir)
    {
        case 'a':
            return LEFT;
        case 'd':
            return RIGHT;
        case 'w':
            return UP;
        case 's':
            return DOWN;
    }
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
        case LEFT:
            snake.body[0].x--;
            break;
        case RIGHT:
            snake.body[0].x++;
            break;
        case UP:
            snake.body[0].y--;
            break;
        case DOWN:
            snake.body[0].y++;
            break;
    }
}

void applyGravity()
{
    while (1) {
        // Fall out of map
        if (snake.body[0].y > 32) {
            uart_puts("Game Over\n");
            return;
        }

        for (int i = 0; i < snake.length; i++)
        {
            for (int j = 0; j < MAX_BRICKS; j++) 
            {
                if (map_one.bricks[j].x <= snake.body[i].x && snake.body[i].x <= map_one.bricks[j].x + map_one.bricks[j].w - 1)
                {
                    if (snake.body[i].y == map_one.bricks[j].y - 1)
                    {
                        return;
                    }
                }
            }
        }

        for (int i = 0; i < snake.length; i++)
        {
            snake.body[i].y += 1;
        }
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
        int x = map_one.bricks[i].x;
        int y = map_one.bricks[i].y;
        // int length = map_one.bricks[i].length;

        // // Horizontal brick
        // if (map_one.bricks[i].direction == 0)
        // {
        //     if (snake.body[0].y == y - 1)
        //     {
        //         if ((snake.body[0].x + snake.length) - 1 < x || (x + length) < snake.body[0].x)
        //         {
        //             uart_puts("out brick\n");
        //         }
        //         else
        //         {
        //             uart_puts("on brick\n");
        //             snake.onBrick = 1;
        //             break;
        //         }
        //     }
        // }
        // // Vertical brick
        // else
        // {
            
        // }
        
    }
}

void playGame()
{
    initializeGame();
    wait_ms(5000);

    while (1)
    {
        char c = uart_getc();
        int direction = getDirection(c);
        
        if (isValidMove(direction)) {
            moveSnake();
        }
        applyGravity();
        checkCollision();
        checkOnBrick();

        clearScreen(0x0);

    
        drawBricks();
        drawSnake();
        drawFood();
    }
}