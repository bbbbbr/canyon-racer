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

#ifndef common_H
#define common_H

#include <gbdk/bcd.h>

// These are required for calculations below
#include "../res/sprite_ship.h"
#include "../res/sprite_obstacles.h"
#include "../res/tiles_font_nums.h"

// Needed for obstacle array size def
#include "entity_obstacles.h"
// Needed for cur_level
#include "level.h"

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define SECONDS_IN_FRAMES(sec) (sec * 60u)

extern uint8_t decomp_buf[];

#define BUTTON__JUMP          (J_A)
#define BUTTON__PAUSE         (J_START)
#define BUTTON__STATE_SAVE    (J_B)


// GB Sound macros
#define AUDTERM_ALL_LEFT  (AUDTERM_4_LEFT | AUDTERM_3_LEFT | AUDTERM_2_LEFT | AUDTERM_1_LEFT)
#define AUDTERM_ALL_RIGHT (AUDTERM_4_RIGHT | AUDTERM_3_RIGHT | AUDTERM_2_RIGHT | AUDTERM_1_RIGHT)



#define GAME_STATE_STARTUP_CREDITS   0u
#define GAME_STATE_SHOW_INTRO        1u
#define GAME_STATE_SHOW_HELP         2u
#define GAME_STATE_START_GAME        3u
#define GAME_STATE_RUNNING           4u
#define GAME_STATE_OVER              5u


// ===========================

// #define VISUAL_DEBUG_BENCHMARK_MAIN
// #define VISUAL_DEBUG_BENCHMARK_AUDIO

// Debug option to resume gameplay after crashing
// #define DEBUG_RESUME_AFTER_CRASH

// Debug option to show level in score
// #define DEBUG_SHOW_LEVEL_IN_SCORE

// #define DEBUG_MUSIC_IS_OFF

// #define DEBUG_FAST_LEVELS_EARLY

// #define DEBUG_SKIP_INTRO_CREDITS

// #define DEBUG_COLLISION_HITBOXES

// ===========================

#define DMG_MGB_LESS_BLUR_STYLE

#define TILE_SIZE_PX   8u

#define FIXED_BITSHIFT 8

// SHIP
#define SPR_TILES_START_SHIP 0u
#define SPR_TILES_COUNT_SHIP_MAX   (sprite_ship_TILE_COUNT - 1u)

#define SPR_TILES_START_OBSTACLES (SPR_TILES_COUNT_SHIP_MAX + 1u)
#define SPR_TILES_COUNT_OBSTACLES_MAX   ((SPR_TILES_START_OBSTACLES + sprite_obstacles_TILE_COUNT) - 1u)


#define SPR_TILES_START_FONTNUMS (SPR_TILES_COUNT_OBSTACLES_MAX + 1u)
#define SPR_TILES_COUNT_FONTNUMS_MAX   ((SPR_TILES_START_FONTNUMS + tiles_font_nums_TILE_COUNT) - 1u)
#define SPR_TILES_FONTNUMS_DIGIT_0 (SPR_TILES_START_FONTNUMS) // first digit is zero
#define SPR_TILES_FONTNUMS_DIGIT_R (SPR_TILES_START_FONTNUMS + (10u * 2u)) // Extra character trailing after the digits x 2 for (2 tiles per 8x16 sprite)


// Single "R" for Restore indicator before text
#define SPR_ID_LIVES_HEADER_START 0u
#define SPR_ID_LIVES_HEADER_COUNT 1u
#define SPR_ID_LIVES_HEADER_MAX   (SPR_ID_LIVES_HEADER_START + SPR_ID_LIVES_HEADER_COUNT - 1u)

#define SPR_ID_LIVES_DISPLAY_START (SPR_ID_LIVES_HEADER_MAX + 1u)
#define SPR_ID_LIVES_DISPLAY_COUNT 2u
#define SPR_ID_LIVES_DISPLAY_MAX   (SPR_ID_LIVES_DISPLAY_START + SPR_ID_LIVES_DISPLAY_COUNT - 1u)


#define SPR_ID_SCORE_START (SPR_ID_LIVES_DISPLAY_MAX + 1u)
#define SPR_ID_SCORE_COUNT 5u  // Score is now truncated to 5 digits // 6u
#define SPR_ID_SCORE_MAX   (SPR_ID_SCORE_START + SPR_ID_SCORE_COUNT - 1u)

#define SPR_ID_FREE_START  (SPR_ID_SCORE_MAX + 1u)

#define SPR_ID_MAX 39u  // GB supports up to 40 sprites, 0 based index


// CANYON X Axis offsets
#define CANYON_LEFT_X_BASE    (8u * 8u) // 8 tiles x 8 pixels per tile
#define CANYON_WIDTH          (4u * 8u) // 4 tiles x 8 pixels per tile


#define GAME_OVER_WAIT_SECONDS 0.5

// Save record signature check
#define STATS_SIG_CHECK_0 0xA50Fu
#define STATS_SIG_CHECK_1 0x1E78u

// Struct containing the save game
// For MBC5 will be mapped to the SRAM through a pointer
typedef struct settings_rec {

    // Don't change order, it will scramble cart saves
    uint8_t  save_checksum;
    uint16_t save_check0;
    uint16_t save_check1;
    // Stats
    BCD hi_score;
    bool help_never_shown;

        // Not implemented, set to zero on first run
        uint8_t opt_blank_1;
        uint8_t opt_blank_2;
        uint8_t opt_blank_3;
        uint8_t opt_blank_4;
        uint8_t opt_blank_5;
        uint8_t opt_blank_6;
        uint8_t opt_blank_7;
        uint8_t opt_blank_8;

} settings_rec;


typedef struct obs_ent {
    fixed   y;
    uint8_t type;
} obs_ent;


typedef struct game_state_data {
    // Map FX
        // **MUST** BE FIRST ITEM. DO NOT MOVE OR IT BREAKS ASM ADDRESSING (+ 0) to it
        const uint8_t * p_scx_table_scanline;

    // Gameplay
        bool paused;

    // Level
        uint8_t      game_level;            // Current game level
        uint8_t      level_count_till_next; // Obstacles to clear until level increment
        level_entry  cur_level;             // Settings data for current level
        uint8_t      level_scx_speed_prev;


    // Ship
        fixed   ship_x, ship_y;
        fixed   ship_z;
        uint8_t ship_state;
        uint8_t ship_counter;
        int16_t ship_jump_velocity;

    // Obstacles
        obs_ent obstacles[OBSTACLES_COUNT_TOTAL];
        // == Difficulty level related ==
            // See struct level_entry {}, level_entry settings
        // == Active entities related
        uint8_t obstacles_active_first;
        uint8_t obstacles_active_last;
        uint8_t obstacles_active_count;

        uint8_t obstacles_next_countdown;
        uint8_t obstacles_next_type;
        uint8_t obstacles_next_isdouble;
        uint8_t obstacles_item_queued;

    // Common
        settings_rec game_settings;

    // Map FX
        const uint8_t * p_scx_table_frame_base;
        const uint8_t * p_scx_table_stop;
        const uint8_t * p_scx_cur_table;

        uint8_t   mapfx_y_parallax_speed;
        uint8_t   mapfx_scx_table_map_speed;
        // Matching vars used for save during pause/resume
        uint8_t   save__mapfx_y_parallax_speed;
        uint8_t   save__mapfx_scx_table_map_speed;

        uint8_t   mapfx_level_mask;
        uint8_t   mapfx_level_base;

    // Score
        BCD score;

} game_state_data;

extern game_state_data state;

void delay_lowcpu(uint16_t num_frames);
void wait_in_halt_to_scanline(uint8_t exit_scanline);

void game_state_save(void);
void game_state_restore(void);

#endif