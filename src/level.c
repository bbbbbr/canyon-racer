#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 #include <rand.h>

#include <gb/isr.h>
#include "common.h"

#include "level.h"

#include "map_fx.h"
#include "lookup_tables.h"
#include "entity_obstacles.h"


static uint8_t  game_level;            // Current game level
        uint8_t level_count_till_next; // Obstacles to clear until level increment

typedef struct level_entry {
    uint8_t  mapfx;
    uint8_t  obst_qty_max;
    uint8_t  obst_dist_min;
} level_entry;

// mapfx should not exceed SCX_TABLE_LEVEL_MAX
// obst_qty_max should not exceed OBSTACLES_COUNT_TOTAL


// TODO: adjustable obstacle speed?
// TODO: adjustable obstacle distance range mask
// TODO: max speed
// TODO
    // #define OBSTACLE_INC_SPEED_MIN    450u
    // #define OBSTACLE_INC_SPEED_MAX    597u



static const level_entry level_data[] = {
    {.mapfx = MAPFX_LVL_EASY,   .obst_qty_max = OBST_QTY_LVL_EASY, .obst_dist_min = OBST_DIST_MIN_LVL_EASY},
    {.mapfx = MAPFX_LVL_MED_LO, .obst_qty_max = OBST_QTY_LVL_MED,  .obst_dist_min = OBST_DIST_MIN_LVL_MED },
    {.mapfx = MAPFX_LVL_MED,    .obst_qty_max = OBST_QTY_LVL_MED,  .obst_dist_min = OBST_DIST_MIN_LVL_MED },
    {.mapfx = MAPFX_LVL_MED_HI, .obst_qty_max = OBST_QTY_LVL_HARD, .obst_dist_min = OBST_DIST_MIN_LVL_HARD},
    {.mapfx = MAPFX_LVL_HARD,   .obst_qty_max = OBST_QTY_LVL_HARD, .obst_dist_min = OBST_DIST_MIN_LVL_HARD},

// TODO make tiers of levels where first tier is few objects


};

#define GAME_LEVEL_RESET  0u
#define GAME_LEVEL_MAX    (ARRAY_LEN(level_data) - 1u)


// OPTIMIZE: inline using global var access if needed
void level_update_vars(void) {

    // TODO:
    // - obstacle speed
    // - obstacle up/down
    // - obstacle range
    // - BG parallax speed?
    // - Level increment threshold dial in
    //   - More leel tiers

    // Map Canyon Shape difficulty
    mapfx_level_set(level_data[game_level].mapfx);

    // Obstacle Speed and Quantity difficult
    obstacles_level_set(level_data[game_level].obst_qty_max, level_data[game_level].obst_dist_min);
}


void level_init(void) {

    level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT_RESET;
    game_level = GAME_LEVEL_RESET;

    // Debug:
    // game_level = GAME_LEVEL_MAX;

    level_update_vars();
}


void level_increment(void) {

    level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT;

    if (game_level < GAME_LEVEL_MAX)
        game_level++;

    level_update_vars();

    // TODO: play sound / flash notice / etc on level up
}
