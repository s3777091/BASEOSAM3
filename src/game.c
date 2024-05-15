#include "./lib/game.h"

struct Snake snake;
struct Teleport teleport;

int currentMap = 0; // Track current map index

void initializeGame()
{
    initializeMap(currentMap);
    wait_ms(5000);
    snake.length = 3;
    snake.direction = LEFT; // Initial direction: left
    snake.onBrick = 0;

    for (int i = 0; i < snake.length; i++)
    {
        snake.body[i].x = map.start.x - i;
        snake.body[i].y = map.start.y;
    }
}

void drawSnake()
{
    for (int i = 0; i < snake.length; i++)
    {
        int color = (i == 0) ? 0x6 : 0x2; // Assuming 0x6 represents pink
        drawRect(snake.body[i].x * CELL_SIZE, snake.body[i].y * CELL_SIZE,
                 (snake.body[i].x * CELL_SIZE) + CELL_SIZE - 1,
                 (snake.body[i].y * CELL_SIZE) + CELL_SIZE - 1, color, 1);
    }
}

void drawFood()
{
    for (int i = 0; i < MAX_APPLES; i++)
    {
        if (map.apples[i].x != -1 && map.apples[i].y != -1)
        {
            int appleX = map.apples[i].x * CELL_SIZE;
            int appleY = map.apples[i].y * CELL_SIZE;

            drawRect(appleX, appleY, appleX + CELL_SIZE, appleY + CELL_SIZE, 0x4, 1);
        }
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

int isValidMove(int move) {
    int newX = snake.body[0].x;
    int newY = snake.body[0].y;

    switch (move) {
    case LEFT:
        newX--;
        break;
    case RIGHT:
        newX++;
        break;
    case UP:
        newY--;
        break;
    case DOWN:
        newY++;
        break;
    default:
        return 0; // Invalid move input
    }

    int isValid = 1;
    snake.onBrick = 0;

    // Combine brick and apple checks into one loop
    for (int i = 0; i < MAX_BRICKS; i++) {
        int brickLeft = map.bricks[i].x;
        int brickRight = map.bricks[i].x + map.bricks[i].w - 1;
        int brickTop = map.bricks[i].y;
        int brickBottom = map.bricks[i].y + map.bricks[i].h - 1;

        if (newX >= brickLeft && newX <= brickRight && newY >= brickTop && newY <= brickBottom) {
            snake.onBrick = 1;  // Snake is on a brick
            isValid = 0;        // Move is not valid if it hits a brick
        }
    }

    for (int i = 1; i < snake.length; i++) {
        if (newX == snake.body[i].x && newY == snake.body[i].y) {
            uart_puts("Struggle! Snake hit itself!\n");
            isValid = 0;  // Move is not valid if it hits itself
        }
    }

    for (int i = 0; i < MAX_APPLES; i++) {
        if (newX == map.apples[i].x && newY == map.apples[i].y) {
            snake.onBrick = 1;  // Snake is on an apple
        }
    }

    if (isValid) {
        snake.direction = move;
    }

    return isValid;
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

void applyGravity() {
    int falling = 1;

    while (falling) {
        if (snake.body[0].y > 32) {
            uart_puts("Game Over\n");
            return;
        }

        int isAboveBrickOrApple = 0;

        for (int i = 0; i < snake.length; i++) {
            for (int j = 0; j < MAX_BRICKS; j++) {
                if (map.bricks[j].x <= snake.body[i].x && snake.body[i].x <= map.bricks[j].x + map.bricks[j].w - 1 &&
                    snake.body[i].y + 1 == map.bricks[j].y) {
                    isAboveBrickOrApple = 1;
                    break;
                }
            }

            if (!isAboveBrickOrApple) {
                for (int j = 0; j < MAX_APPLES; j++) {
                    if (map.apples[j].x == snake.body[i].x && map.apples[j].y == snake.body[i].y + 1) {
                        isAboveBrickOrApple = 1;
                        break;
                    }
                }
            }

            if (isAboveBrickOrApple) {
                falling = 0;
                break;
            }
        }

        if (!falling)
            break;

        for (int i = 0; i < snake.length; i++) {
            snake.body[i].y += 1;
        }

        wait_ms(100);

        clearScreen(0x0);
        drawSnake();
        drawBricks();
        drawTeleport();
        drawFood();
    }
}

void checkCollision() {
    for (int i = 0; i < MAX_APPLES; i++) {
        if (snake.body[0].x == map.apples[i].x && snake.body[0].y == map.apples[i].y) {
            if (snake.length < MAX_SNAKE_LENGTH) {
                int tail_index = snake.length;
                int last_x = snake.body[tail_index - 1].x;
                int last_y = snake.body[tail_index - 1].y;
                int second_last_x = snake.body[tail_index - 2].x;
                int second_last_y = snake.body[tail_index - 2].y;

                if (last_x == second_last_x) {
                    if (last_y > second_last_y) {
                        snake.body[tail_index].y = last_y + 1;
                        snake.body[tail_index].x = last_x;
                    } else {
                        snake.body[tail_index].y = last_y - 1;
                        snake.body[tail_index].x = last_x;
                    }
                } else if (last_y == second_last_y) {
                    if (last_x > second_last_x) {
                        snake.body[tail_index].x = last_x + 1;
                        snake.body[tail_index].y = last_y;
                    } else {
                        snake.body[tail_index].x = last_x - 1;
                        snake.body[tail_index].y = last_y;
                    }
                }

                snake.length++;  // Increase snake length after updating position
            }
            map.apples[i].x = -1;  // Remove apple
            map.apples[i].y = -1;  // Remove apple
        }
    }

    if (snake.body[0].x == map.teleport.x && snake.body[0].y == map.teleport.y) {
        currentMap++;  // Advance to the next map
        uart_puts("Go to next map\n");
        if (currentMap >= MAX_MAPS) {
            currentMap = 0;  // Loop back to the first map
        }
        initializeMap(currentMap);  // Initialize the new map

        snake.length = 3;
        for (int i = 0; i < snake.length; i++) {
            snake.body[i].x = map.start.x - i;
            snake.body[i].y = map.start.y;
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
        int direction = getDirection(c);

        if (isValidMove(direction))
        {
            moveSnake();
        }
        checkCollision();
        applyGravity();

        clearScreen(0x0);

        drawTeleport();
        drawBricks();
        drawSnake();
        drawFood();
    }
}
