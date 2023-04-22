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
#include <gbdk/metasprites.h>
#include <gb/gbdecompress.h>

#include <stdint.h>
#include <stdbool.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"

#include "rotate_screen_effect.h"

#include "../res/sprite_sound_test_cursor.h"

#include "../res/tiles_font_nums_bg.h"

#include "../res/intro_credits_data.h"
#include "../res/intro_credits_map_comp.h"
#include "../res/intro_credits_tiles_comp.h"

#include "sound_test.h"

const uint8_t spr_cursor_offset[] = {0,1,2,3,3,2,1,0};

static       uint8_t sound_menu_counter[SOUND_MENU_NUM_COUNTERS];
static       uint8_t sound_menu_max[SOUND_MENU_NUM_COUNTERS];    // Max value for each counter
static const uint8_t sound_menu_counter_y[] = {SOUND_TEST_COUNTER_Y_SFX, SOUND_TEST_COUNTER_Y_MUSIC, SOUND_TEST_COUNTER_Y_EXIT}; // Y position to render

static int8_t sound_menu_cursor;

static void sound_menu_change(int8_t dir);
static void sound_menu_cursor_change(int8_t dir);
static void sound_menu_cursor_update(void);
static void sound_test_menu(void);


void sound_test_run(void) {

    // NOTE: No need to install audio_vbl_isr, handled by caller (help_screen)

    // Reset scroll in case it's off due to previous background fx
    SCY_REG = 0u;
    set_1bpp_colors(DMG_BLACK, DMG_WHITE);

    // Set up cursor sprite (hide all others)
    HIDE_SPRITES;
    SPRITES_8x8;
    set_sprite_data(0u, sprite_sound_test_cursor_TILE_COUNT, sprite_sound_test_cursor_tiles);
    set_sprite_tile(SPR_SOUND_TEST_CURSOR, 0u);
    hide_sprites_range(0u, MAX_HARDWARE_SPRITES);

    // Sound test menu is hidden at the bottom of the intro credits screen
    // To display it here with the Beatscribe Logo at the top of the screen
    // clip off the bbbbbr skull cat part of the map at the top.
    //
    gb_decompress(intro_credits_tiles_comp, decomp_buf);
    set_bkg_1bpp_data(0u, intro_credits_data_TILE_COUNT, decomp_buf);

    gb_decompress(intro_credits_map_comp, decomp_buf);
    set_bkg_tiles(0u, 32u - (INTRO_CREDITS_BBBBBR_SKULL_CAT_HEIGHT),
                  (intro_credits_data_WIDTH / intro_credits_data_TILE_W),
                  (intro_credits_data_HEIGHT / intro_credits_data_TILE_H),
                  decomp_buf);


    // Load Splash BG Font Num tiles and render menu entries with default value (min setting)
    set_bkg_data(SOUND_TEST_FONT_NUMS_START_TILE, tiles_font_nums_bg_TILE_COUNT, tiles_font_nums_bg_tiles);

    set_bkg_tile_xy(SOUND_TEST_COUNTER_X, SOUND_TEST_COUNTER_Y_SFX,   SOUND_TEST_FONT_NUMS_START_TILE + SOUND_TEST_COUNTER_MIN_VAL);
    set_bkg_tile_xy(SOUND_TEST_COUNTER_X, SOUND_TEST_COUNTER_Y_MUSIC, SOUND_TEST_FONT_NUMS_START_TILE + SOUND_TEST_COUNTER_MIN_VAL);

    // Installs LCD and VBL ISRs
    rotate_screen_effect_init(SOUND_TEST_BLANK_LINE_FOR_HIDING);

    // Fades in and then runs until effect completes,
    // then disables LCD ISR and removes VBL ISR
    fade_in(FADE_DELAY_SOUND_TEST_IN);
    rotate_screen_effect_run();

    sound_test_menu();

    audio_start_fadeout();
    // Screen fade out time needs to be long enough for audio fade-out to compelte
    fade_out(FADE_DELAY_SOUND_TEST_OUT);

    // Return to standard sprite mode for rest of the game
    SPRITES_8x16;

    // NOTE: No need to remove audio_vbl_isr, handled by caller (help_screen)
}


// Increments or decrements a menu item counter
static void sound_menu_change(int8_t dir) {

    // Abort if cursor is not on a counter (i.e. on Exit)
    if (sound_menu_cursor == SOUND_MENU_IDX_EXIT)
        return;

    // Load menu item to work with
    int8_t newval = sound_menu_counter[sound_menu_cursor];
    int8_t maxval = sound_menu_max[sound_menu_cursor];

    newval += dir;

    if (newval < SOUND_TEST_COUNTER_MIN_VAL)
        newval = maxval;
    else if (newval > maxval)
        newval = SOUND_TEST_COUNTER_MIN_VAL;

    // Save updated value
    sound_menu_counter[sound_menu_cursor] = newval;

    set_bkg_tile_xy(SOUND_TEST_COUNTER_X, sound_menu_counter_y[sound_menu_cursor], SOUND_TEST_FONT_NUMS_START_TILE + newval);
}


// Move the cursor to another menu entry
static void sound_menu_cursor_change(int8_t dir) {
    sound_menu_cursor += dir;

    if (sound_menu_cursor < 0)
        sound_menu_cursor = SOUND_MENU_MAX;
    else if (sound_menu_cursor > SOUND_MENU_MAX)
        sound_menu_cursor = 0;

    // SFX for moving the cursor is a little distracting when other sounds are playing
    // audio_sfx_play(SFX_SOUNDTEST_CURSOR);
}


// Update cursor on the screen
static void sound_menu_cursor_update(void) {

    // Animate the cursor with a mild bounce
    move_sprite(SPR_SOUND_TEST_CURSOR, SOUND_TEST_CURSOR_X,
                (sound_menu_counter_y[sound_menu_cursor] * TILE_SIZE_PX)
                  + (DEVICE_SPRITE_PX_OFFSET_Y + SOUND_CURSOR_ADJUST_Y)
                  + spr_cursor_offset[(sys_time & SOUND_CURSOR_LUT_MASK) >> SOUND_CURSOR_BITSHIFT]);
}


static void sound_test_menu(void) {

    // Init default values for menu item counters
    sound_menu_counter[SOUND_MENU_IDX_SFX]   = SOUND_TEST_COUNTER_MIN_VAL;
    sound_menu_counter[SOUND_MENU_IDX_MUSIC] = SOUND_TEST_COUNTER_MIN_VAL;
    sound_menu_max[SOUND_MENU_IDX_SFX]       = sfx_list_max + SOUND_TEST_COUNTER_MIN_VAL;
    sound_menu_max[SOUND_MENU_IDX_MUSIC]     = music_list_max + SOUND_TEST_COUNTER_MIN_VAL;

    // Start cursor on SFX menu item
    sound_menu_cursor = SOUND_MENU_IDX_SFX;

    // Move cursor to initial position and turn on sprites
    SHOW_SPRITES;

    while (1) {

        vsync();

        sound_menu_cursor_update();

        UPDATE_KEYS();
        if (KEY_TICKED(J_ANY)) {
            switch (KEYS()) {
                // Change selected menu item up/down
                case J_UP:;
                    sound_menu_cursor_change(SOUND_TEST_CURSOR_UP);
                    break;

                case J_DOWN:
                    sound_menu_cursor_change(SOUND_TEST_CURSOR_DOWN);
                    break;

                // Pauses Music
                case J_B:
                    audio_music_pause();
                    break;

                // Plays selected SFX/Music/Exits
                case J_A:
                case J_START:
                    // Plays SFX, Toggles music On/Off
                    if (sound_menu_cursor == SOUND_MENU_IDX_SFX)
                        audio_sfx_play(sound_menu_counter[SOUND_MENU_IDX_SFX] - SOUND_TEST_COUNTER_MIN_VAL);
                        // TODO: Fixme: MUSIC 1 PLAYING (title) -> Play SFX 6 (speed up) -> leaves 1 channel muted for a while
                    else if (sound_menu_cursor == SOUND_MENU_IDX_MUSIC) {
                        audio_music_set(sound_menu_counter[SOUND_MENU_IDX_MUSIC] - SOUND_TEST_COUNTER_MIN_VAL);
                        audio_music_unpause();
                    }
                    else
                        return; // "Exit" chosen, quit sound test

                    break;

                // Change selected counter
                case J_LEFT:
                    sound_menu_change(SOUND_TEST_MENU_DEC);
                    break;

                case J_RIGHT:
                    sound_menu_change(SOUND_TEST_MENU_INC);
                    break;
            }
        }
    }
}

