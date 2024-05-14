#include "./lib/map.h"

struct Map map;

// x, y, w, h
struct Map all_maps[4] = {
    {
        {{7, 13, 10, 1}, {12, 10, 1, 3}}, // Bricks
        {{10, 11}}, // Apples
        {16, 12} // Teleport
    },
    {
        {{7, 13, 10, 1}}, // Bricks
        {{10, 10}, {10, 20}}, // Apples
        {8, 12} // Teleport
    }

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
    map.teleport = src->teleport;
}