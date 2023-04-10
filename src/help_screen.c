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

#include "help_screen_effect.h"

#include "../res/help_screen_data.h"
#include "../res/help_screen_map_comp.h"
#include "../res/help_screen_tiles_comp.h"


void help_screen_run(void) {

    HIDE_SPRITES;

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
    help_effect_init();

    // Fades in and then runs until effect completes,
    // then disables LCD ISR and removes VBL ISR
    fade_in(FADE_DELAY_HELP_IN);
    help_effect_run();

    waitpadticked_lowcpu(J_ANY);
    fade_out(FADE_DELAY_HELP_OUT);
}
