#ifndef MAP_H
#define MAP_H

#define MAX_BRICKS 12
#define MAX_APPLES 3
#define MAX_ROCKS 3
#define CELL_SIZE 32
#define MAX_MAPS 10

#include "game.h"

struct Start {
    int x, y;
};

struct Brick {
    int x, y, w, h;
};

struct Apple {
    int x, y;
};

struct Rock {
    int x, y;
};

struct Teleport {
    int x, y;
};

struct Map {
    struct Brick bricks[MAX_BRICKS];
    struct Apple apples[MAX_APPLES];
    struct Rock rocks[MAX_ROCKS];
    struct Teleport teleport;
    struct Start start;
};

extern struct Map all_maps[MAX_MAPS];
extern struct Map map;

void initializeMap(int level);

void drawFood();
void drawSnake();
void drawTeleport();
void drawBricks();
void drawRocks();

#endif // MAP_H
