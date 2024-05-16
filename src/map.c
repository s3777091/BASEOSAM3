#include "./lib/map.h"
#include "./lib/image.h"

// Map structure
struct Map map;

// x, y, w, h
struct Map all_maps[MAX_MAPS] = {
    {
        {{10, 13, 5, 1}, {20, 13, 5, 1}}, // Bricks
        {{16, 11}}, // Apples
        {{12, 11}}, // rock
        {24, 12}, // Teleport
        {12, 12}, // start
    },
    {
        {{10, 13, 2, 1}, {10, 14, 1, 2}, {10, 16, 5, 1}, {14, 14, 1, 2},  {13, 13, 2, 1}, {16, 13, 1, 1}}, // Bricks
        {{12, 15}},
        {{-1, -1}},
        {16, 14},
        {11, 11},
    },
    {
        {{10, 13, 7, 1}, {17, 13, 1, 2}, {17, 14, 3, 1}, {19, 10, 1, 4}, {19, 10, 3, 1}, {16, 10, 1, 1}},
        {{15, 7}},
        {{12, 11}},
        {20, 11},
        {12, 11},
    },
    {
        {{10, 13, 5, 1}, {13, 9, 1, 3}, {18, 13, 3, 1}, {17, 11, 1, 1}, {16, 9, 2, 1}}, // Bricks
        {{15, 11}},
        {{-1, -1}},
        {20, 14},
        {11, 12},
    },
    
    // Rock Map
};

void initializeMap(int level) {
    int idx = level % MAX_MAPS;  // Wrap around the map index
    struct Map *src = &all_maps[idx];

    for (int i = 0; i < MAX_BRICKS; i++) {
        map.bricks[i] = src->bricks[i];
    }
    for (int i = 0; i < MAX_APPLES; i++) {
        map.apples[i] = src->apples[i];
    }

    for (int i = 0; i < MAX_ROCKS; i++) {
        map.rocks[i] = src->rocks[i];
    }

    map.start = src->start;
    map.teleport = src->teleport;
}

// Function to draw the snake
void drawSnake()
{
    for (int i = 0; i < snake.length; i++)
    {
        if (i == 0)
        {
            drawImageTiled(snakeHead, 
                           snake.body[i].x * CELL_SIZE, 
                           snake.body[i].y * CELL_SIZE, 
                           CELL_SIZE, CELL_SIZE);
        }
        else
        {
            drawImageTiled(snakeBody, 
                           snake.body[i].x * CELL_SIZE, 
                           snake.body[i].y * CELL_SIZE, 
                           CELL_SIZE, CELL_SIZE);
        }
    }
}


// Function to draw the food (apples)
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

// Function to draw the teleport
void drawTeleport()
{
    // Ensure the teleport is drawn using global map teleport coordinates
    drawRect(map.teleport.x * CELL_SIZE,
             map.teleport.y * CELL_SIZE,
             (map.teleport.x + 1) * CELL_SIZE - 1,
             (map.teleport.y + 1) * CELL_SIZE - 1, 0x3, 1);
}

void drawBricks() {
    for (int i = 0; i < MAX_BRICKS; i++) {
        struct Brick temp = map.bricks[i];
        int brickWidth = temp.w * CELL_SIZE;
        int brickHeight = temp.h * CELL_SIZE;
        drawImageTiled(brickImage, temp.x * CELL_SIZE, temp.y * CELL_SIZE, brickWidth, brickHeight);
    }
}

void drawRocks()
{
    for (int i = 0; i < MAX_ROCKS; i++)
    {
        if (map.rocks[i].x != -1 && map.rocks[i].y != -1)
        {
            int rockX = map.rocks[i].x * CELL_SIZE;
            int rockY = map.rocks[i].y * CELL_SIZE;

            drawRect(rockX, rockY, rockX + CELL_SIZE, rockY + CELL_SIZE, 0x7, 1); // Assuming 0x7 represents a color for rocks
        }
    }
}
