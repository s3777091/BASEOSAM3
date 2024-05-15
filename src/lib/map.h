#ifndef MAP_H
#define MAP_H

#define MAX_BRICKS 10
#define MAX_APPLES 5
#define CELL_SIZE 32
#define MAX_MAPS 10


struct Brick {
    int x, y, w, h;
};

struct Apple {
    int x, y;
};

struct Teleport {
    int x, y;
};

struct Map {
    struct Brick bricks[MAX_BRICKS];
    struct Apple apples[MAX_APPLES];
    struct Teleport teleport;
};


extern struct Map all_maps[MAX_MAPS];
extern struct Map map;

void initializeMap(int level);

#endif // MAP_H
