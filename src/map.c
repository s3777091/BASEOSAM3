#include "./lib/map.h"

struct Map map;

// x, y, w, h
struct Map all_maps[MAX_MAPS] = {
    {
        {{7, 13, 10, 1}, {12, 10, 1, 3}}, // Bricks
        {{10, 11}}, // Apples
        {16, 12}, // Teleport
        {8, 12},
    },
    {
        {{10, 13, 5, 1}, {18, 13, 5, 1}},
        {{15, 11}},
        {22, 12},
        {12, 12},
    },
    {
        {{10, 13, 7, 1}, {17, 13, 1, 2}, {17, 14, 3, 1}, {19, 10, 1, 4}, {19, 10, 3, 1}, {16, 10, 1, 2}, {16, 10, 2, 1}},
        {{17, 11}},
        {21, 9},
        {12, 11},
    },
    {
        {{10, 13, 2, 1}, {10, 14, 1, 2}, {10, 16, 5, 1}, {14, 14, 1, 2},  {13, 13, 2, 1}, {16, 13, 1, 1}}, // Bricks
        {{12, 15}}, // Apples
        {16, 14}, // Teleport
        {11, 11},
    },
    {
        {{10, 13, 5, 1}, {13, 9, 1, 3}, {18, 13, 3, 1}, {17, 11, 1, 1}, {16, 9, 2, 1}}, // Bricks
        {{15, 11}}, // Apples
        {20, 14}, // Teleport
        {11, 12},
    },





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

    map.start = src->start;
    map.teleport = src->teleport;
}

