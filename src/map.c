#include "map.h"

struct Map map_one;

void initializeMap()
{
    // Playarea walls
    map_one.bricks[0] = (struct Brick){0, 0, 31, 1};
    map_one.bricks[1] = (struct Brick){0, 23, 32, 1};
    map_one.bricks[2] = (struct Brick){0, 0, 1, 23};
    map_one.bricks[3] = (struct Brick){31, 0, 1, 23};

    map_one.bricks[4] = (struct Brick){16, 13, 3, 1};


    // Define apples (on bricks)
    map_one.apples[0] = (struct Apple){10 * CELL_SIZE, 10 * CELL_SIZE};
    map_one.apples[1] = (struct Apple){20 * CELL_SIZE, 20 * CELL_SIZE};

    // Define teleport (end of first brick)
    // map_one.teleport = (struct Teleport){
    //     (map_one.bricks[0].x + map_one.bricks[0].length - 1) * CELL_SIZE,  (map_one.bricks[0].y - 1) * CELL_SIZE
    // };
}