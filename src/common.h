
#ifndef common_H
#define common_H

#include "../res/sprite_ship.h"
#include "../res/sprite_obstacles.h"
#include "../res/tiles_font_nums.h"


// #define DEBUG_BENCHMARK_BG

#define FIXED_BITSHIFT 8


// SHIP
#define SPR_TILES_START_SHIP 0
#define SPR_TILES_COUNT_SHIP_MAX   (sprite_ship_TILE_COUNT - 1u)

#define SPR_TILES_START_OBSTACLES (SPR_TILES_COUNT_SHIP_MAX + 1u)
#define SPR_TILES_COUNT_OBSTACLES_MAX   ((SPR_TILES_START_OBSTACLES + sprite_obstacles_TILE_COUNT) - 1u)


#define SPR_TILES_START_FONTNUMS (SPR_TILES_COUNT_OBSTACLES_MAX + 1u)
#define SPR_TILES_COUNT_FONTNUMS_MAX   ((SPR_TILES_START_FONTNUMS + tiles_font_nums_TILE_COUNT) - 1u)
#define SPR_TILES_FONTNUMS_DIGIT_0 (SPR_TILES_START_FONTNUMS) // first digit is zero


#define SPR_ID_SCORE_START 0u
#define SPR_ID_SCORE_COUNT 6u
#define SPR_ID_SCORE_MAX   (SPR_ID_SCORE_START + SPR_ID_SCORE_COUNT - 1u)

#define SPR_ID_FREE_START  (SPR_ID_SCORE_MAX + 1u)

#define SPR_ID_MAX 39u  // GB supports up to 40 sprites, 0 based index


// CANYON X Axis offsets
#define CANYON_LEFT_X_BASE    (8u * 8u) // 8 tiles x 8 pixels per tile // ((160u  - 10u) / 2u) // TODO
#define CANYON_WIDTH          (4u * 8u) // 4 tiles x 8 pixels per tile

#endif