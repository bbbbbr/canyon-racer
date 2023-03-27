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
#include <rand.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"
#include "stats.h"
#include <cartsave.h>

#include "magic_code.h"
#include "intro_credits.h"
#include "splash_screen.h"
#include "gameplay.h"
#include "gameover_screen.h"
#include "help_screen.h"
#include "map_fx.h"


uint8_t bg_next_free_tile;
uint8_t spr_next_free_tile;
uint8_t game_state;

bool return_to_intro;

void init_gfx(void) {

    fade_out(FADE_DELAY_NORM);

    SPRITES_8x16;
    SHOW_SPRITES;
    SHOW_BKG;

    DISPLAY_ON;
}


void init(void) {

    #if defined(CART_mbc5) || defined(CART_mbc5_rumble)
        // Initialize MBC bank defaults
        // Upper ROM bank to 1, And SRAM/XRAM bank to 0
        SWITCH_ROM_MBC5(1);
        SWITCH_RAM(MBC_RAM_BANK_0);
        DISABLE_RAM_MBC5;
    #endif

    // if (_cpu == CGB_TYPE) {
        // Don't use CGB 2x mode, it throws off vertical parallax timing (unless an alternate ISR is implemented)
        // Use 2x CGB speed if available
        // cpu_fast();
    // }


    stats_load();
    init_gfx();

    // Call this BEFORE mapfx_isr_install() to ensure audio is configured before it's VBL starts
    audio_init();

    mapfx_set_intro();

    magic_code_reset();
}



void main() {

    init();
    game_state = GAME_STATE_STARTUP_CREDITS;

    while (1) {

        switch (game_state) {

            case GAME_STATE_STARTUP_CREDITS:
                // Fades in, shows credits + plays music, fades out
                #ifndef DEBUG_SKIP_INTRO_CREDITS
                    intro_credits_show();
                #endif
                game_state = GAME_STATE_SHOW_INTRO;
                break;

            case GAME_STATE_SHOW_HELP:
                // Expects: screen palettes faded-out. Wipes out BG graphics. No Music right now
                help_screen_run();
                if (return_to_intro)
                    game_state = GAME_STATE_SHOW_INTRO;
                else
                    game_state = GAME_STATE_START_GAME;

                break;

            case GAME_STATE_SHOW_INTRO:
                HIDE_SPRITES;
                // Expects: screen palettes faded-out
                // Reloads map FX without VBL ISR. Then unloads it again on state done

                // Load up shared BG canyon GFX
                bg_next_free_tile = init_gfx_bg_mapfx(0u);  // Load BG gfx starting at tile 0
                uint8_t return_button = splash_intro_run(bg_next_free_tile);

                return_to_intro = false; // Default to starting game
                // Auto show help on very first game start or when user presses A button
                if (return_button == J_A) {
                    game_state = GAME_STATE_SHOW_HELP;
                    return_to_intro = true;
                }
                else if (return_button == J_START) {
                    if (state.game_settings.help_never_shown)
                        game_state = GAME_STATE_SHOW_HELP;
                    else
                        game_state = GAME_STATE_START_GAME;
                }
                break;

            case GAME_STATE_START_GAME:
                // Expects: screen palettes faded-out

                mapfx_isr_install(MAPFX_AUDIO_VBL_YES);
                spr_next_free_tile = init_gfx_sprites_gameplay(0u);  // Load Sprite gfx starting at tile 0
                bg_next_free_tile = init_gfx_bg_mapfx(0u);  // Load BG gfx starting at tile 0
                SHOW_SPRITES;
                #ifdef DEBUG_SOUND_TEST
                    audio_music_set(song_test_counter);
                #else
                    audio_music_set(MUSIC_GAMEPLAY);
                #endif
                audio_music_unpause();
                gameplay_prestart();
                game_state = GAME_STATE_RUNNING;
                break;

            case GAME_STATE_RUNNING:
                gameplay_run(spr_next_free_tile);
                game_state = GAME_STATE_OVER;
                break;

            case GAME_STATE_OVER:
                gameover_screen_show(bg_next_free_tile);
                game_state = GAME_STATE_SHOW_INTRO;
                // Audio and Screen are faded out at this point
                delay_lowcpu( SECONDS_IN_FRAMES(GAME_OVER_WAIT_SECONDS) );
                // Remove effect from running after fade-out, it will get reinstalled differently for splash screen
                mapfx_isr_deinstall();
                break;
        }
    }
}
