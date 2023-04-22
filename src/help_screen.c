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

#include <gb/gbdecompress.h>

#include <audio.h>

#include "input.h"
#include "common.h"
#include "gfx.h"
#include "fade.h"
#include "cartsave.h"
#include "stats.h"

#include "sound_test.h"

#include "rotate_screen_effect.h"

#include "../res/help_screen_data.h"
#include "../res/help_screen_map_comp.h"
#include "../res/help_screen_tiles_comp.h"

#define HELP_SCREEN_BLANK_LINE_FOR_HIDING 10u // All black scanline for hiding in help screen map

void help_screen_run(void) {

    HIDE_SPRITES;

    __critical {
         add_VBL(audio_vbl_isr);
    }

    // Clear flag that shows help very first time user plays game
    if (state.game_settings.help_never_shown) {
        state.game_settings.help_never_shown = false;
        #if defined(CART_31k_1kflash) || defined(CART_mbc5) || defined(CART_mbc5_rumble)
            cartsave_save_data();
        #endif
    }

    // Reset scroll in case it's off due to previous background fx
    SCY_REG = 0u;
    set_1bpp_colors(DMG_BLACK, DMG_WHITE);

    // == Compressed 1bpp assets version ==
    // 1bpp tiles and map versions of gb_decompress.* would be nice
    gb_decompress(help_screen_tiles_comp, decomp_buf);
    set_bkg_1bpp_data(0, help_screen_data_TILE_COUNT, decomp_buf);

    gb_decompress(help_screen_map_comp, decomp_buf);
    set_bkg_tiles(0,0,
                  (help_screen_data_WIDTH / help_screen_data_TILE_W),
                  (help_screen_data_HEIGHT / help_screen_data_TILE_H),
                  decomp_buf);

    // Installs LCD and VBL ISRs
    rotate_screen_effect_init(HELP_SCREEN_BLANK_LINE_FOR_HIDING);

    // Fades in and then runs until effect completes,
    // then disables LCD ISR and removes VBL ISR
    fade_in(FADE_DELAY_HELP_IN);
    rotate_screen_effect_run();

    bool open_sound_test = false;
    bool loop_run = true;

    while(loop_run) {
        // waitpadticked_lowcpu(J_ANY);
        vsync();

        UPDATE_KEYS();

        if (KEY_PRESSED(J_START | J_A | J_B | J_UP)) {
            // All key presses except SELECT exit the help screen after processing below
            loop_run = false; // Exit
            switch (KEYS()) {

                // Enter sound test menu
                case (J_SELECT | J_B):
                    open_sound_test = true;
                    break;

                // Hold Select + Press Start to reset high-score
                case (J_SELECT | J_UP):
                    // Then de-install audio vbl isr
                    audio_sfx_play(SFX_RESET_HISCORE);

                    stats_hi_score_reset();
                    break;

            }
        }

    }
    fade_out(FADE_DELAY_HELP_OUT);

    if (open_sound_test)
        sound_test_run();

    __critical {
         remove_VBL(audio_vbl_isr);
    }
}
