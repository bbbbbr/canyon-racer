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
#include <string.h>

#include "../common.h"


// Map a secondary stats struct to the beginning of SRAM (0xA000) when using MBC5
settings_rec __at (0xA000) sram_stats;


void cartsave_restore_data(void) {

    ENABLE_RAM_MBC5;
    memcpy((void *)&state.game_settings, (void *)&sram_stats, sizeof(state.game_settings));
    DISABLE_RAM_MBC5;
}


// TODO: warning on failure to save?
void cartsave_save_data(void) {

    ENABLE_RAM_MBC5;
    memcpy((void *)&sram_stats, (void *)&state.game_settings, sizeof(state.game_settings));
    DISABLE_RAM_MBC5;
}



