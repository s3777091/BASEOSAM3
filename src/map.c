#include "map.h"

struct Map map_one;

void initializeMap()
{
    // Define bricks (centered horizontally)
    map_one.bricks[0] = (struct Brick){10, 20, 15, 0}; // Horizontal brick
    map_one.bricks[1] = (struct Brick){10, 40, 25, 1}; // Vertical brick

    // Define apples (on bricks)
    map_one.apples[0] = (struct Apple){21 * CELL_SIZE, 14 * CELL_SIZE};
    map_one.apples[1] = (struct Apple){42 * CELL_SIZE, 24 * CELL_SIZE};

    // Define teleport (end of first brick)
    map_one.teleport = (struct Teleport){
        (map_one.bricks[0].x + map_one.bricks[0].length - 1) * CELL_SIZE,  (map_one.bricks[0].y - 1) * CELL_SIZE
    };
}