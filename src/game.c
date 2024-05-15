#include "./lib/game.h"

struct Snake snake;
struct Teleport teleport;

int currentMap = 0; // Track current map index

void initializeGame()
{
    initializeMap(currentMap);
    wait_ms(5000);
    snake.length = 3;
    snake.direction = LEFT; // Initial direction: right

    // Place snake above the first brick
    for (int i = 0; i < snake.length; i++)
    {
        snake.body[i].x = map.bricks[0].x;
        snake.body[i].y = map.bricks[0].y - 1;
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
        int appleX = map.apples[i].x * CELL_SIZE;
        int appleY = map.apples[i].y * CELL_SIZE;

        drawRect(appleX, appleY, appleX + CELL_SIZE, appleY + CELL_SIZE, 0x4, 1);
    }
}

void drawTeleport()
{
    // Ensure the teleport is drawn using global map teleport coordinates
    drawRect(map.teleport.x * CELL_SIZE,
             map.teleport.y * CELL_SIZE,
             (map.teleport.x + 1) * CELL_SIZE - 1,
             (map.teleport.y + 1) * CELL_SIZE - 1, 0x3, 1);
}

void drawBricks()
{
    for (int i = 0; i < MAX_BRICKS; i++)
    {
        struct Brick temp = map.bricks[i];
        drawRect(temp.x * CELL_SIZE, temp.y * CELL_SIZE,
                 (temp.x + temp.w) * CELL_SIZE,
                 (temp.y + temp.h) * CELL_SIZE, 0x1, 1);
    }
}

int isValidMove(int move)
{
    int newX = snake.body[0].x;
    int newY = snake.body[0].y;

    // Determine the new head position based on the proposed direction
    switch (move)
    {
    case LEFT:
        if (snake.direction != RIGHT) newX--;
        break;
    case RIGHT:
        if (snake.direction != LEFT) newX++;
        break;
    case UP:
        if (snake.direction != DOWN) newY--;
        break;
    case DOWN:
        if (snake.direction != UP) newY++;
        break;
    default:
        return 0; // Invalid move input
    }

    // Check if the new head position would collide with any brick
    for (int i = 0; i < MAX_BRICKS; i++)
    {
        // Calculate the exact boundaries of each brick
        int brickLeft = map.bricks[i].x;
        int brickRight = map.bricks[i].x + map.bricks[i].w - 1;
        int brickTop = map.bricks[i].y;
        int brickBottom = map.bricks[i].y + map.bricks[i].h - 1;

        // Check for a collision
        if (newX >= brickLeft && newX <= brickRight && newY >= brickTop && newY <= brickBottom)
        {
            return 0; // Collision detected, move is not valid
        }
    }

    // If no collision and move is not in the opposite direction of current, it's valid
    if ((move == LEFT && snake.direction != RIGHT) ||
        (move == RIGHT && snake.direction != LEFT) ||
        (move == UP && snake.direction != DOWN) ||
        (move == DOWN && snake.direction != UP))
    {
        snake.direction = move;
        return 1; // No collision, move is valid
    }

    return 0; // If direction was opposite, move is not valid
}


int getDirection(char dir)
{
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

    return 0;
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
    // Continue falling until landing on a brick or reaching the map limit
    int falling = 1;

    while (falling)
    {
        // Check if the head of the snake is falling out of the map
        if (snake.body[0].y > 32)
        {
            uart_puts("Game Over\n");
            return;
        }

        // Check each segment of the snake for collision with bricks
        for (int i = 0; i < snake.length; i++)
        {
            for (int j = 0; j < MAX_BRICKS; j++)
            {
                if (map.bricks[j].x <= snake.body[i].x && snake.body[i].x <= map.bricks[j].x + map.bricks[j].w - 1)
                {
                    if (snake.body[i].y + 1 == map.bricks[j].y) // Adjusted to check the position snake will be after falling
                    {
                        falling = 0; // Stop the loop if any part of the snake is above a brick
                        break;
                    }
                }
            }
            if (!falling)
                break; // Exit early if a collision is found
        }

        if (!falling)
            continue; // If a collision is detected, skip the falling

        for (int i = 0; i < snake.length; i++)
        {
            snake.body[i].y += 1;
        }

        wait_ms(100);

        clearScreen(0x0);
        drawSnake();
        drawBricks();
        drawTeleport();
    }
}

void checkCollision()
{
    // Check if snake eats the food
    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (snake.body[0].x == map.apples[i].x &&
            snake.body[0].y == map.apples[i].y)
        {
            if (snake.length < MAX_SNAKE_LENGTH)
            {
                snake.length++;
            }
            map.apples[i].x = -1; // Remove apple
            map.apples[i].y = -1; // Remove apple
        }
    }

    if (snake.body[0].x == map.teleport.x && snake.body[0].y == map.teleport.y)
    {
        currentMap++; // Advance to the next map
        uart_puts("Go to next map\n");
        if (currentMap >= MAX_MAPS)
        {
            currentMap = 0; // Loop back to the first map
        }
        initializeMap(currentMap); // Initialize the new map
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

        if (isValidMove(direction))
        {
            moveSnake();
        }
        applyGravity();
        checkCollision();

        clearScreen(0x0);

        drawTeleport();
        drawBricks();
        drawSnake();
        drawFood();
    }
}