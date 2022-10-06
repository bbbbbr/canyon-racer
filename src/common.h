
#ifndef common_H
#define common_H

#include <gbdk/bcd.h>

// These are required for calculations below
#include "../res/sprite_ship.h"
#include "../res/sprite_obstacles.h"
#include "../res/tiles_font_nums.h"

#define ARRAY_LEN(A)  sizeof(A) / sizeof(A[0])

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#define SECONDS_IN_FRAMES(sec) (sec * 60u)

#define GAME_STATE_SHOW_INTRO 0u
#define GAME_STATE_START_GAME 1u
#define GAME_STATE_RUNNING    2u
#define GAME_STATE_OVER       3u

// ===========================

// #define VISUAL_DEBUG_BENCHMARK_MAIN
// #define VISUAL_DEBUG_BENCHMARK_AUDIO

// Debug option to resume gameplay after crashing
// #define DEBUG_RESUME_AFTER_CRASH

// Debug option to show level in score
#define DEBUG_SHOW_LEVEL_IN_SCORE

#define DEBUG_SOUND_TEST

// #define DEBUG_MUSIC_IS_OFF

// #define DEBUG_FAST_LEVELS_EARLY

#define DEBUG_SKIP_INTRO_CREDITS

// ===========================

// #define DMG_MGB_LESS_BLUR_STYLE


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

extern settings_rec game_settings;

void delay_lowcpu(uint16_t num_frames);
void wait_in_halt_to_scanline(uint8_t exit_scanline);

#endif