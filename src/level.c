// Copyright 2023 (c) bbbbbr
//
// This software is licensed under:
//
// CC-BY-NC-SA: Attribution Non-Commercial Share Alike Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International License
// See: http://creativecommons.org/licenses/by-nc-sa/4.0/
//
// For the purposes of this project "Share Alike" shall also include
// distribution of the source code and any changes to it.

#include <gbdk/platform.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
 #include <rand.h>

#include <gb/isr.h>
#include "common.h"
#include "audio.h"

#include "level.h"

#include "map_fx.h"
#include "lookup_tables.h"
#include "entity_obstacles.h"


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

// DEBUG:
#ifdef DEBUG_FAST_LEVELS_EARLY
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_FAST, OBST_QTY_LVL_MED,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_FAST, OBST_QTY_LVL_MED,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_HI, MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
#endif

    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM, OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM, OBST_QTY_LVL_EASY, OBST_DIST_MIN_MED,  OBST_INC_SPD_EASY),
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_FAST, OBST_QTY_LVL_EASY, OBST_DIST_MIN_EASY, OBST_INC_SPD_EASY),

    // 5-9: MAP_SCX: Slow, OBST_SPD=Medium
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    LEVEL_ENTRY(MAPFX_LVL_MED_HI, MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED,  OBST_INC_SPD_MED),
    // LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_HARD, OBST_INC_SPD_MED_HARD),
    // LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_HARD, OBST_INC_SPD_MED_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED, OBST_INC_SPD_MED_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_NORM, OBST_QTY_LVL_MED,  OBST_DIST_MIN_MED, OBST_INC_SPD_MED_HARD),

    // 10-15: MAP_SCX: Fast, OBST_SPD=Fast - Easier obj distance due to high speed
    LEVEL_ENTRY(MAPFX_LVL_EASY,   MAPFX_SCX_FAST, OBST_QTY_LVL_MED,  OBST_DIST_MIN_EASY, OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_LO, MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED,    MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_MED_HI, MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED,  OBST_INC_SPD_HARD),
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_MED, OBST_INC_SPD_HARD),
    // LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_HARD, OBST_INC_SPD_HARD),

    // The final level, carries on to infinity
    LEVEL_ENTRY(MAPFX_LVL_HARD,   MAPFX_SCX_FAST, OBST_QTY_LVL_HARD, OBST_DIST_MIN_HARD2, OBST_INC_SPD_HARD),

};

#define GAME_LEVEL_RESET  0u
#define GAME_LEVEL_MAX    (ARRAY_LEN(level_data) - 1u)


// OPTIMIZE: inline using global var access if needed
void level_update_vars(void) {

    // Update state.cur_level to new level data (includes obstacle state.cur_level)
    state.cur_level = level_data[state.game_level];

    // Min distance between next spawned object
    state.cur_level.obst_dist_min = OBST_COUNT_FROM_SCANLINES( state.cur_level.obst_dist_min_raw, state.cur_level.obst_speed);
    // Distance apart when double objects are spawned
    state.cur_level.obst_dist_double = OBST_COUNT_FROM_SCANLINES( OBST_DIST_DBL, state.cur_level.obst_speed);

    // Update Map Canyon Shape difficulty
    mapfx_level_set(state.cur_level.mapfx_scx_table_level);
}


void level_init(void) {

    state.level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT_RESET;
    state.game_level = GAME_LEVEL_RESET;

    // DEBUG:
    // state.game_level = GAME_LEVEL_MAX;

    level_update_vars();

    // Init saved SCX wave scrolling speed
    state.level_scx_speed_prev = state.cur_level.mapfx_scx_inc_every_frame;
}


void level_increment(void) {

    state.level_count_till_next = LEVEL_OBSTACLES_TILL_NEXT;

    if (state.game_level < GAME_LEVEL_MAX) {

        state.game_level++;

        // Save SCX wave scrolling speed for comparison later
        state.level_scx_speed_prev = state.cur_level.mapfx_scx_inc_every_frame;
        level_update_vars();
    }

    // Play "speed up" sound instead of level up if
    // scx wave scrolling increased in speed
    if (state.level_scx_speed_prev < state.cur_level.mapfx_scx_inc_every_frame)
        audio_sfx_play(SFX_SPEED_UP);
    else
        audio_sfx_play(SFX_LEVEL_UP);

    // TODO: flash notice / etc on level up
    //  - giving objects a chance to clear off the screen maybe? - straight segment - with timeout?
}
