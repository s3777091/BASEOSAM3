#include "./lib/map.h"
#include "./lib/image.h"
#include "./lib/framebf.h"

// Map structure
struct Map map;

// x, y, w, h
struct Map all_maps[MAX_MAPS] = {
    {
        {{7, 13, 10, 1}, {12, 10, 1, 3}},
        {{10, 11}},
        {{-1, -1}},
        {16, 12},
        {8, 12},
    },
    {
        {{10, 13, 5, 1}, {20, 13, 5, 1}}, // Bricks
        {{16, 11}},                       // Apples
        {{12, 11}},                       // rock
        {24, 12},                         // Teleport
        {12, 12},                         // start
    },
    {
        {{7, 8, 3, 1}, {9, 9, 1, 3}, {10, 11, 2, 1}, {11, 10, 3, 1}, {13, 11, 1, 2}, {14, 12, 2, 1}, {15, 8, 1, 4}, {11, 8, 3, 1}},
        {{12, 9}},
        {{-1, -1}},
        {17, 5},
        {9, 7},
    },
    {
        {{10, 13, 5, 1}, {13, 9, 1, 3}, {18, 13, 3, 1}, {17, 11, 1, 1}, {16, 9, 2, 1}}, // Bricks
        {{15, 11}},
        {{-1, -1}},
        {20, 14},
        {11, 12},
    },

    {
        {{10, 13, 7, 1}, {17, 13, 1, 2}, {17, 14, 3, 1}, {19, 10, 1, 4}, {19, 10, 3, 1}, {16, 10, 1, 2}, {16, 10, 2, 1}},
        {{17, 11}},
        {{-1, -1}},
        {21, 9},
        {12, 11},
    },

    {
        {{10, 13, 2, 1}, {10, 14, 1, 2}, {10, 16, 5, 1}, {14, 14, 1, 2}, {13, 13, 2, 1}, {16, 13, 1, 1}}, // Bricks
        {{12, 15}},
        {{-1, -1}},
        {16, 14},
        {11, 11},
    },
    {
        {{10, 13, 5, 1}, {13, 9, 1, 3}, {18, 13, 3, 1}, {17, 11, 1, 1}, {16, 9, 2, 1}}, // Bricks
        {{15, 11}},
        {{-1, -1}},
        {20, 14},
        {11, 12},
    },
    {{{7, 10, 4, 1}, {7, 11, 1, 2}, {10, 11, 1, 1}, {7, 13, 4, 1}, {14, 13, 3, 1}, {16, 10, 1, 3}},
     {{12, 12}},
     {{-1, -1}},
     {16, 9},
     {9, 9}},
    {{{6, 15, 1, 2}, {7, 16, 2, 1}, {8, 15, 7, 1}, {15, 16, 4, 1}, {18, 12, 1, 4}, {8, 12, 5, 1}, {10, 7, 1, 5}},
     {{7, 13}},
     {{8, 11}, {13, 11}},
     {18, 11},
     {12, 14}},

    {{{7, 8, 4, 1}, {8, 9, 3, 1}, {8, 10, 3, 1}, {8, 11, 1, 3}, {9, 13, 1, 2}, {10, 14, 3, 1}, {12, 13, 3, 1}, {14, 8, 1, 5}, {12, 8, 2, 1}, {12, 9, 2, 1}, {12, 10, 2, 1}, {15, 8, 1, 1}},
     {{10, 12}},
     {{12, 12}, {10, 11}},
     {15, 9},
     {9, 7}}

    // Rock Map
};

void initializeMap(int level)
{
    int idx = level % MAX_MAPS; // Wrap around the map index
    struct Map *src = &all_maps[idx];

    for (int i = 0; i < MAX_BRICKS; i++)
    {
        map.bricks[i] = src->bricks[i];
    }
    for (int i = 0; i < MAX_APPLES; i++)
    {
        map.apples[i] = src->apples[i];
    }

    for (int i = 0; i < MAX_ROCKS; i++)
    {
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

// Function to draw the bricks
void drawBricks()
{
    for (int i = 0; i < MAX_BRICKS; i++)
    {
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

void drawBackground(unsigned int *img, uint32_t char_x, uint32_t char_y, uint32_t widthDraw, uint32_t heightDraw)
{
    uint32_t x1 = char_x;
    uint32_t y1 = char_y;
    uint32_t x2 = char_x + widthDraw;
    uint32_t y2 = char_y + heightDraw;

    // for (int y = y1; y < y2; y++)
    //     for (int x = x1; x < x2; x++, img++)
    //         drawPixelARGB32(x, y, *img);
    drawImage(img, x1, y1, x2, y2);
}

void MapReload()
{
    clearScreen(0x0);
    drawBackground(backgroundIMG, 0, 0, 1024, 768);
    drawTeleport();
    drawBricks();
    drawSnake();
    drawFood();
    drawRocks();
}