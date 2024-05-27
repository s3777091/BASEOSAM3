#include "./lib/game.h"

// Init Global variables
struct Snake snake;
struct Teleport teleport;
int currentMap = 0; // Track current map index
int isRunning = 1;

/*
    Initialize the game
*/
void initializeGame() {
    initializeMap(currentMap); // Load map
    wait_ms(5000);
    snake.length = 3;       // Init snake length
    snake.direction = LEFT; // Initial direction: left
    snake.onBrick = 0;
    isRunning = 1;

    // Init snake body position
    for (int i = 0; i < snake.length; i++) {
        snake.body[i].x = map.start.x - i;
        snake.body[i].y = map.start.y;
    }
}

/*
    Check if the move is valid
    return 1 for true and 0 for false
*/
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

    // Check for collisions with bricks, rocks, and the snake itself
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

    // Check if snake hits itself
    for (int i = 1; i < snake.length; i++) {
        if (newX == snake.body[i].x && newY == snake.body[i].y) {
            uart_puts("Struggle! Snake hit itself!\n");
            isValid = 0;  // Move is not valid if it hits itself
        }
    }

    // Check if the snake eats apples
    for (int i = 0; i < MAX_APPLES; i++) {
        if (newX == map.apples[i].x && newY == map.apples[i].y) {
            snake.onBrick = 1;  // Snake is on an apple
        }
    }

    for (int i = 0; i < MAX_ROCKS; i++) {
        if (newX == map.rocks[i].x && newY == map.rocks[i].y) {
            // Move the rock
            int rockNewX = map.rocks[i].x;
            int rockNewY = map.rocks[i].y;

            switch (move) {
            case LEFT:
                rockNewX--;
                break;
            case RIGHT:
                rockNewX++;
                break;
            case UP:
                rockNewY--;
                break;
            case DOWN:
                rockNewY++;
                break;
            }

            // Check if the new rock position is valid
            int rockValid = 1;
            for (int j = 0; j < MAX_BRICKS; j++) {
                int brickLeft = map.bricks[j].x;
                int brickRight = map.bricks[j].x + map.bricks[j].w - 1;
                int brickTop = map.bricks[j].y;
                int brickBottom = map.bricks[j].y + map.bricks[j].h - 1;

                if (rockNewX >= brickLeft && rockNewX <= brickRight &&
                    rockNewY >= brickTop && rockNewY <= brickBottom) {
                    rockValid = 0;  // Rock cannot be moved to a position inside a brick
                    break;
                }
            }

            // Check if the new rock position collides with other rocks
            for (int j = 0; j < MAX_ROCKS; j++) {
                if (rockNewX == map.rocks[j].x && rockNewY == map.rocks[j].y) {
                    rockValid = 0;  // Rock cannot be moved to a position occupied by another rock
                    break;
                }
            }

            // Check if the new rock position collides with an apple
            for (int j = 0; j < MAX_APPLES; j++) {
                if (rockNewX == map.apples[j].x && rockNewY == map.apples[j].y) {
                    rockValid = 0;
                    break;
                }
            }

            if (rockValid) {
                map.rocks[i].x = rockNewX;
                map.rocks[i].y = rockNewY;
            } else {
                isValid = 0;  // Snake move is invalid if the rock cannot be moved
            }
        }
    }

    // Make the move if it is valid
    if (isValid) {
        snake.direction = move;
    }

    return isValid;
}

/*
    Convert character into specific directions
*/ 
int getDirection(char dir) {
    switch (dir) {
        case 'a':
            return LEFT;
        case 'd':
            return RIGHT;
        case 'w':
            return UP;
        case 's':
            return DOWN;
    }

    return -1;
}

/*
    Move the snake to destinated position    
*/
void moveSnake() {
    // Move the snake in the current direction
    for (int i = snake.length - 1; i > 0; i--) {
        snake.body[i] = snake.body[i - 1];
    }

    switch (snake.direction) {
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

/*

*/
void applyGravity() {
    while (1) {
        if (snake.body[0].y > BOUND_Y) {
            uart_puts("\nGame Over\n");
            uart_puts("Press R to restart\n");
            uart_puts("Press Q to quit\n");
            return;
        }

        if (snake.body[0].x == map.teleport.x && snake.body[0].y == map.teleport.y) {
            uart_puts("Go to next map\n");
            advanceToNextMap();
        }

        for (int i = 0; i < snake.length; i++) {
            int x = snake.body[i].x;
            int y = snake.body[i].y + 1; // Check the position below the snake segment

            // Check for bricks, apples, or rocks below the snake
            for (int j = 0; j < MAX_BRICKS; j++) {
                if (x >= map.bricks[j].x && x <= map.bricks[j].x + map.bricks[j].w - 1 &&
                    y == map.bricks[j].y) {
                    return;
                }
            }

            for (int j = 0; j < MAX_APPLES; j++) {
                if (x == map.apples[j].x && y == map.apples[j].y) {
                    return;
                }
            }

            for (int j = 0; j < MAX_ROCKS; j++) {
                if (x == map.rocks[j].x && y == map.rocks[j].y) {
                    return;
                }
            }
        }

        // Move the snake down by 1 cell
        for (int i = 0; i < snake.length; i++) {
            snake.body[i].y += 1;
        }
    }
}

/*
    Check collision
*/
void checkCollision() {
    for (int i = 0; i < MAX_APPLES; i++) {
        if (snake.body[0].x == map.apples[i].x && snake.body[0].y == map.apples[i].y) {
            if (snake.length < MAX_SNAKE_LENGTH) {
                int tail_index = snake.length;
                int last_x = snake.body[tail_index - 1].x;
                int last_y = snake.body[tail_index - 1].y;

                int new_tail_x, new_tail_y;
                int found_valid_position = 0;

                // Check all four directions around the last segment
                int possible_positions[4][2] = {
                    {last_x + 1, last_y},
                    {last_x - 1, last_y},
                    {last_x, last_y + 1},
                    {last_x, last_y - 1}
                };

                for (int j = 0; j < 4; j++) {
                    new_tail_x = possible_positions[j][0];
                    new_tail_y = possible_positions[j][1];

                    // Check if the new position is within the bounds
                    if (new_tail_x < 0 || new_tail_x >= BOUND_X || new_tail_y < 0 || new_tail_y >= BOUND_Y) {
                        continue;
                    }

                    // Check if the new position collides with a brick or rock
                    int is_valid = 1;

                    for (int k = 0; k < MAX_BRICKS; k++) {
                        if (new_tail_x >= map.bricks[k].x && new_tail_x <= map.bricks[k].x + map.bricks[k].w - 1 &&
                            new_tail_y >= map.bricks[k].y && new_tail_y <= map.bricks[k].y + map.bricks[k].h - 1) {
                            is_valid = 0;
                            break;
                        }
                    }
                    if (!is_valid) continue;

                    for (int k = 0; k < MAX_ROCKS; k++) {
                        if (new_tail_x == map.rocks[k].x && new_tail_y == map.rocks[k].y) {
                            is_valid = 0;
                            break;
                        }
                    }

                    if (is_valid) {
                        found_valid_position = 1;
                        break;
                    }
                }

                if (!found_valid_position) {
                    uart_puts("Game Over: No valid position for the new tail segment.\n");
                    return;
                }

                // Set the new tail position
                snake.body[tail_index].x = new_tail_x;
                snake.body[tail_index].y = new_tail_y;

                // Increase snake length after updating position
                snake.length++;
            }
            map.apples[i].x = -1;  // Remove apple
            map.apples[i].y = -1;  // Remove apple
        }
    }

    
}

void advanceToNextMap() {
    currentMap++;  // Advance to the next map
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


void playGame() {
    initializeGame();
    wait_ms(5000);
    

    while (1 && isRunning) {
        char c = uart_getc();

        // Exit the game
        if (c == 'q') {
            isRunning = 0;
            return;
        }

        // Reload the map
        if (c == 'r') {
            initializeMap(currentMap);
            snake.length = 3;
            for (int i = 0; i < snake.length; i++) {
                snake.body[i].x = map.start.x - i;
                snake.body[i].y = map.start.y;
            }
        }

        int direction = getDirection(c);

        if (isValidMove(direction)) {
            moveSnake();
        }
        checkCollision();
        applyGravity();

        MapReload();
    }
}
