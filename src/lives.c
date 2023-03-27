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
#include <gbdk/bcd.h>

#include "common.h"
#include "magic_code.h"
#include "lives.h"

BCD lives_count;                                    // AKA "Rewind"
const BCD lives_count_bcd_step_size = MAKE_BCD(01); // 1 in BCD


// Reset number of lives to default
void lives_count_reset(void) {
    if (IS_MAGIC_CODE_ACTIVE)
        lives_count = LIVES_COUNT_CHEAT;
    else
        lives_count = LIVES_COUNT_RESET;
}


// Increment number of lives, limit to max value
void lives_count_increment(void) {
    // Only need to test lsbyte, no need for full BCD test since max is 99
    if ( *(uint8_t *)&(lives_count) != (uint8_t)LIVES_COUNT_MAX_BCD) {
        LIVES_COUNT_ADD_ONE();
    }
}


// Displays number of lives left using sprites
void lives_display_update(void) {

    // Pointer to LSByte of State Restore Count BCD value
    uint8_t * p_bcd_pair = (uint8_t *) &lives_count;

    // Render score from Right to Left in digit pairs
    set_sprite_tile(SPR_ID_LIVES_DISPLAY_START + 1u, SPR_TILES_START_FONTNUMS + ((*p_bcd_pair & 0x0Fu) << 1));
    set_sprite_tile(SPR_ID_LIVES_DISPLAY_START + 0u, SPR_TILES_START_FONTNUMS + ((*p_bcd_pair >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost
}
