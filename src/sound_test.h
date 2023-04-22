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

#ifndef _SOUND_TEST_H
#define _SOUND_TEST_H

#define SOUND_TEST_BLANK_LINE_FOR_HIDING 130u // All black scanline for hiding in help screen map

// Cursor settings
#define SPR_SOUND_TEST_CURSOR        0u
#define SOUND_TEST_CURSOR_X          ((5u * TILE_SIZE_PX) + DEVICE_SPRITE_PX_OFFSET_X)
// #define SOUND_TEST_CURSOR_X          ((6u * TILE_SIZE_PX) + DEVICE_SPRITE_PX_OFFSET_X)

#define SOUND_TEST_CURSOR_UP   -1
#define SOUND_TEST_CURSOR_DOWN  1

#define SOUND_CURSOR_UPDATE_MASK 0x03 // Only update cursor once every 8 frames
#define SOUND_CURSOR_BITSHIFT    2
#define SOUND_CURSOR_LUT_MASK    0x07 << SOUND_CURSOR_BITSHIFT // 8 offset entries in the LUT
#define SOUND_CURSOR_ADJUST_Y    -2

#define SOUND_TEST_MENU_DEC    -1
#define SOUND_TEST_MENU_INC     1

// Menu item readout numbers X position
#define SOUND_TEST_COUNTER_X         (13u)

// Menu item positions and settings
#define SOUND_TEST_COUNTER_Y_SFX     (11u)
#define SOUND_TEST_COUNTER_Y_MUSIC   (13u)
#define SOUND_TEST_COUNTER_Y_EXIT    (15u)
// #define SOUND_TEST_COUNTER_X         (11u)
// #define SOUND_TEST_COUNTER_Y_SFX     (12u)
// #define SOUND_TEST_COUNTER_Y_MUSIC   (SOUND_TEST_COUNTER_Y_SFX + 1u)
// #define SOUND_TEST_COUNTER_Y_EXIT    (SOUND_TEST_COUNTER_Y_MUSIC + 1u)

#define SOUND_MENU_IDX_SFX          0u
#define SOUND_MENU_IDX_MUSIC        (SOUND_MENU_IDX_SFX + 1u)

#define SOUND_MENU_NUM_COUNTER_MAX  (SOUND_MENU_IDX_MUSIC)
#define SOUND_MENU_NUM_COUNTERS     (SOUND_MENU_NUM_COUNTER_MAX + 1u)

#define SOUND_MENU_IDX_EXIT         (SOUND_MENU_IDX_MUSIC + 1u)
#define SOUND_MENU_MAX              (SOUND_MENU_IDX_EXIT)

#define INTRO_CREDITS_BBBBBR_SKULL_CAT_HEIGHT    (8u)  // 8 tiles tall
#define SOUND_TEST_FONT_NUMS_START_TILE (intro_credits_data_TILE_COUNT)

#define SOUND_TEST_COUNTER_MIN_VAL  1u

void sound_test_run(void);

#endif
