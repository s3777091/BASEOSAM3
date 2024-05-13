#ifndef MAP_H
#define MAP_H

#define MAX_BRICKS 10
#define MAX_APPLES 5
#define CELL_SIZE 20
// Structures for map elements
struct Brick
{
    int length;
    int x;
    int y;
    int direction;  // 0: horizontal, 1: vertical
};

struct Apple
{
    int x;
    int y;
};

struct Teleport
{
    int x;
    int y;
};

// Map definition
struct Map
{
    struct Brick bricks[MAX_BRICKS];
    struct Apple apples[MAX_APPLES];
    struct Teleport teleport;
};

extern struct Map map_one;

void initializeMap();

#endif // MAP_H
