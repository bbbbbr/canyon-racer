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


uint8_t game_level; // Current game level
uint8_t level_count_till_next; // Obstacles to clear until level increment

level_entry cur_level;


// "scanlines" is pre-scaled with OBS_COUNT_SCALE_UP(), which multiplies by 256
#define OBST_COUNT_FROM_SCANLINES(scanlines_pre_scaled_raw, obs_inc_speed) (scanlines_pre_scaled_raw / obs_inc_speed)

#define LEVEL_ENTRY(MAPFX_ENTRY, MAPFX_FAST_SCX, OBST_QTY_MAX, OBST_DIST_MIN, OBST_SPEED) \
    {.mapfx_scx_table_level     = MAPFX_ENTRY, \
     .mapfx_scx_inc_every_frame = MAPFX_FAST_SCX, \
     .obst_qty_max              = OBST_QTY_MAX, \
     .obst_dist_min_raw         = OBST_DIST_MIN, \
     .obst_speed                = OBST_SPEED}


static const level_entry level_data[] = {
    // 0 - 4: MAP_SCX: Slow, OBST_SPD=Slow
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_NORM,  OBST_QTY_LVL_EASY, OBST_DIST_MIN_EASY, OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_NORM,  OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM,  OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM,  OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_NORM,  OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),

    // 5-9: MAP_SCX: Slow, OBST_SPD=Medium
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM,  OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_NORM,  OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    LEVEL_ENTRY(MAPFX_LVL_MED_HI, MAPFX_SCX_NORM,  OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM,  OBST_QTY_LVL_MED, OBST_DIST_MIN_HARD, OBST_INC_SPD_MED_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM,  OBST_QTY_LVL_MED, OBST_DIST_MIN_HARD, OBST_INC_SPD_MED_HARD),

    // 10-15: MAP_SCX: Fast, OBST_SPD=Fast - Easier obj distance due to high speed
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_FAST, OBST_QTY_LVL_MED,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_FAST, OBST_QTY_LVL_MED,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_FAST, OBST_QTY_LVL_HARD,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_HI, MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),

};

#define GAME_LEVEL_RESET  0u
#define GAME_LEVEL_MAX    (ARRAY_LEN(level_data) - 1u)



// OPTIMIZE: inline using global var access if needed
void level_update_vars(void) {

    // TODO:
    // - obstacle range mask
    // - Level increment threshold dial in
    // - obstacle up/down (floating / hidden / blacking?)

    // Update cur_level to new level data (includes obstacle cur_level)
    cur_level = level_data[game_level];

    // Min distance between next spawned object
    cur_level.obst_dist_min = OBST_COUNT_FROM_SCANLINES( cur_level.obst_dist_min_raw, cur_level.obst_speed);
    // Distance apart when double objects are spawned
    cur_level.obst_dist_double = OBST_COUNT_FROM_SCANLINES( OBST_DIST_DBL, cur_level.obst_speed);


    // Update Map Canyon Shape difficulty
    mapfx_level_set(cur_level.mapfx_scx_table_level);

}


void level_init(void) {

    level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT_RESET;
    game_level = GAME_LEVEL_RESET;

    // TODO: FOR DEBUG
    // game_level = GAME_LEVEL_MAX;
    // game_level = GAME_LEVEL_MAX - 6;
    // game_level = 9; // 

    level_update_vars();
}


void level_increment(void) {

    level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT;

    if (game_level < GAME_LEVEL_MAX) {

        game_level++;
        level_update_vars();
    }

    // TODO: play sound / flash notice / etc on level up
}