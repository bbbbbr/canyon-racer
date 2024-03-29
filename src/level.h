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

#ifndef LEVEL_H
#define LEVEL_H

#include <stdint.h>
#include <stdbool.h>

#define LEVEL_OBSTACLES_TILL_NEXT  10u // Gets hard a lot faster  // 20u is more chill
#define LEVEL_OBSTACLES_TILL_NEXT_RESET (LEVEL_OBSTACLES_TILL_NEXT)


typedef struct level_entry {

    // Map FX
    uint8_t   mapfx_scx_table_level;       // Used to select which range of SCX table segments to for a given difficulty level
    bool      mapfx_scx_inc_every_frame;   // Whether current scx wave table is scrolled every frame, or every other frame

    // Obstacles
    uint8_t   obst_qty_max;     // Controls max number of spawned obstacles
    uint16_t  obst_speed;       // Speed of obstacles incremented per frame

    uint8_t   obst_dist_min;    // Min distance between next spawned objext
    uint8_t   obst_dist_double; // Set distance when double objects are spawned (always recalc based on obstacle speed)

    uint16_t   obst_dist_min_raw;    // Value before scaling by speed

} level_entry;


void level_update_vars(void);
void level_init(void);
void level_increment(void);


#define LEVEL_INC_AND_CHECK() state.level_count_till_next--; \
                              if (state.level_count_till_next == 0u) { level_increment(); }


#endif
