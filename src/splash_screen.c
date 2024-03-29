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
#include <stdint.h>
#include <stdbool.h>

#include <audio.h>

// Graphics
#include "../res/splash_logo_data.h"
#include "../res/splash_logo_map_comp.h"
#include "../res/splash_logo_tiles_comp.h"

#include "../res/tiles_font_nums_bg.h"

#include <gb/gbdecompress.h>

#include "input.h"
#include "common.h"
#include "fade.h"
#include "score.h"

#include "magic_code.h"
#include "map_fx.h"

#include "splash_screen.h"

#define WIN_X_OFFSET   7u // GameBoy Window hardware X offset
#define WIN_X_FINAL    0u // Hide almost one column off-screen
#define WIN_X_INITIAL  0u // Same as final, but gets changed almost instantly

#define WIN_SPEED_SLOW 1u
#define WIN_SPEED_MED  2u
#define WIN_SPEED_FAST 4u

#define SPLASH_LOGO_TITLE_ONLY_HEIGHT (13u * 8u) // 13 tiles tall (12 + 1 line of help text)
#define WIN_Y_SHOWING_TITLE ((SCREENHEIGHT) - (SPLASH_LOGO_TITLE_ONLY_HEIGHT))

// 36 scanlines should be ok for servicing music
// (takes as little as 16, sometimes spikes to an
// entire frame [less 3/4 of cpu used for LCD ISR],
// might be wave or pattern reload?)
#define TITLE_SCREEN_MUSIC_TIME_IN_SCANLINES  36u
#define WIN_Y_UPDATE_MUSIC_AT ((SCREENHEIGHT) - (TITLE_SCREEN_MUSIC_TIME_IN_SCANLINES))
#define WIN_Y_OFFSCREEN     ((SCREENHEIGHT) + 1u)

#define WIN_MOVE_DIR_DOWN 0
#define WIN_MOVE_DIR_UP   1
const uint8_t scx_shake_offset[] = {7u, 6u, 5u, 4u, 3u, 2u};

static void wait_vbl_then_midframe_update_music(void);
static uint8_t splash_init(uint8_t bg_next_free_tile);

static void window_move_with_shake(uint8_t, uint8_t);


// Waits till scanline N (below window top in this case)
// to update playing music so that glitches are obscured.
// Wait in HALT state so that ISR timing is predictable.
// The LCD ISR will wake it (so must be running).
static void midframe_update_music_then_waitvbl(void) {

    wait_in_halt_to_scanline(WIN_Y_UPDATE_MUSIC_AT);
    audio_vbl_isr(); // Update playback manually
    vsync();
}


static uint8_t splash_init(uint8_t bg_next_free_tile) {

    mapfx_set_intro();
    // This doesn't accept arbitrary tile count, just decompresses until data is used up
    // Non-compressed version:
    // set_bkg_data(bg_next_free_tile, splash_logo_TILE_COUNT, splash_logo_tiles);
    gb_decompress_bkg_data(bg_next_free_tile, splash_logo_tiles_comp);

    // Splash logo goes on the Window so it can overlay on top of the scrolling background
    // Non-compressed version:
    // set_win_based_tiles(SPLASH_LOGO_WIN_X, SPLASH_LOGO_WIN_Y,
    //                     splash_logo_WIDTH / splash_logo_TILE_W, splash_logo_HEIGHT / splash_logo_TILE_H,
    //                     splash_logo_map, bg_next_free_tile);
    gb_decompress(splash_logo_map_comp, decomp_buf);
    set_win_based_tiles(SPLASH_LOGO_WIN_X, SPLASH_LOGO_WIN_Y,
                        splash_logo_data_WIDTH / splash_logo_data_TILE_W, splash_logo_data_HEIGHT / splash_logo_data_TILE_H,
                        decomp_buf, bg_next_free_tile);

    bg_next_free_tile += splash_logo_data_TILE_COUNT;

    // Load Splash BG Font Num tiles and render score
    set_bkg_data(bg_next_free_tile, tiles_font_nums_bg_TILE_COUNT, tiles_font_nums_bg_tiles);
    hi_score_render(get_win_xy_addr(SPLASH_LOGO_WIN_SCORE_X, SPLASH_LOGO_WIN_SCORE_Y), bg_next_free_tile);

    // Window start off-screen
    WY_REG = (SCREENHEIGHT + 1u);
    WX_REG = WIN_X_INITIAL;
    SHOW_WIN;

    return bg_next_free_tile;
}


// Moves window to desired Y location. Only shakes when moving upward
static void window_move_with_shake(uint8_t win_y_moveto, uint8_t move_dir) {

    // Window destination
    uint8_t scroll_amt;
    uint8_t win_y_delta;

    // Scroll window into view (with a small ease-in)
    while (1) {
        win_y_delta = WY_REG - win_y_moveto;

        if (win_y_delta > 20u)
            scroll_amt =  WIN_SPEED_FAST;
        else if (win_y_delta > 10u)
            scroll_amt = WIN_SPEED_MED;
        else
            scroll_amt = WIN_SPEED_SLOW;

        if (move_dir == WIN_MOVE_DIR_UP) {
            // Give the window progressively smaller X Axis shakes as it
            // approaches the target Y location (scrolling in from the bottom).
            //
            // To prevent the background from showing through during the shakes:
            // * There is a 8 pixel (1 tile) margin on the left side
            // * The Window left edge gradually moves left as the shakes diminish
            //   to ensure it ends up with the logo properly aligned and the
            //   left margin hidden.
            //
            WX_REG = WIN_X_OFFSET + ((DIV_REG >> 4) & (0x1u << (win_y_delta >> 5))) - scx_shake_offset[(win_y_delta >> 5)];
            WY_REG -= scroll_amt;
            if (WY_REG <= win_y_moveto) break;
        }
        else {
            // Implied: WIN_MOVE_DIR_DOWN
            WY_REG += scroll_amt;
            if (WY_REG >= win_y_moveto) break;
        }
        midframe_update_music_then_waitvbl();
    }
    // Make sure Window ends up at desired location
    WX_REG = WIN_X_FINAL;
}


// Expects Sprites to be turned off
uint8_t splash_intro_run(uint8_t bg_next_free_tile) {

    uint8_t return_keys;

    magic_code_reset();

    // Start up effect (Splash screen music is run manually and not by vbl)
    mapfx_isr_install(MAPFX_AUDIO_VBL_NO);

    audio_music_set(MUSIC_TITLE_SCREEN);
    audio_music_unpause();

    bg_next_free_tile = splash_init(bg_next_free_tile);

    fade_in(FADE_DELAY_FX_RUNNING);
    window_move_with_shake(WIN_Y_SHOWING_TITLE, WIN_MOVE_DIR_UP);

    // Idle until user presses any button
    UPDATE_KEYS();
    while (1) {

        midframe_update_music_then_waitvbl();

        UPDATE_KEYS();
        if (magic_code_update()) {
            audio_sfx_play(SFX_MAGIC_CODE);
        }
        else if (KEY_TICKED(J_START | J_A)) {
            return_keys = keys;
            break;
        }
    }
    // Now install the vbl isr so that the exit splash SFX will play
    __critical {
        add_VBL(audio_vbl_isr);
    }

    // No music fade out here since SFX should play immediately
    // and it's harder to hear with it playing
    audio_music_pause();
    audio_sfx_play(SFX_TITLE_EXIT);
    window_move_with_shake(WIN_Y_OFFSCREEN, WIN_MOVE_DIR_DOWN);

    fade_out(FADE_DELAY_FX_RUNNING);

    // Remove effect from running after fade-out
    mapfx_isr_deinstall();

    HIDE_WIN;
    return return_keys;
}


