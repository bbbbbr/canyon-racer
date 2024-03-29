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

#include "common.h"
#include "magic_code.h"
#include "score.h"
#include "stats.h"
#include "level.h"



const BCD score_increment_amt = MAKE_BCD(00010); // Increment by 10 each time

void score_reset(void) {
    if (IS_MAGIC_CODE_ACTIVE)
        state.score = MAKE_BCD(888888);
    else
        state.score = MAKE_BCD(000000);
}


void score_increment(void) {
    // When magic code is active score is forced to 88888 and won't increment
    if (!(IS_MAGIC_CODE_ACTIVE))
        bcd_add(&state.score, &score_increment_amt);
}

// DEBUG - show game level
#ifdef DEBUG_SHOW_LEVEL_IN_SCORE
    extern uint8_t game_level;
#endif

// Displays a score during gameplay with sprites
void score_update(void) {

    // BCD type is a uint32_t
    uint8_t * p_score = (uint8_t *) &state.score;

    // TODO: Delete
    // uint8_t c = SCORE_DIGITS;
    // while (c != 0)  {
    //     // Render two digits into adjacent sprites, right -> left
    //     // Score is stored as packed BCD in a uint32_t, one digit per nybble
    //     // Digits are In memory as 0x10, 0x32, 0x54
    //     // 8x16 sprite mode means tile ID's need to be *= 2
    //     set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score & 0x0Fu) << 1));
    //     set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

    //     // Move to next pair of digits in next byte
    //     p_score++;
    // }

    // Render score from Right to Left in digit pairs
    set_sprite_tile(SPR_ID_SCORE_START + 4u, SPR_TILES_START_FONTNUMS + ((*p_score & 0x0Fu) << 1));
    set_sprite_tile(SPR_ID_SCORE_START + 3u, SPR_TILES_START_FONTNUMS + ((*(p_score++) >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

    set_sprite_tile(SPR_ID_SCORE_START + 2u, SPR_TILES_START_FONTNUMS + ((*p_score & 0x0Fu) << 1));
    set_sprite_tile(SPR_ID_SCORE_START + 1u, SPR_TILES_START_FONTNUMS + ((*(p_score++) >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

    // Since there are only 5 digits, only extract one from MSByte of BCD score
    set_sprite_tile(SPR_ID_SCORE_START + 0u, SPR_TILES_START_FONTNUMS + ((*p_score >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

    // DEBUG - show game level
    #ifdef DEBUG_SHOW_LEVEL_IN_SCORE
        set_sprite_tile(SPR_ID_SCORE_START, SPR_TILES_START_FONTNUMS + (game_level << 1));
    #endif
}



// Displays a score on the intro title screen rendered into BG map tiles
void hi_score_render(uint8_t * p_vram_addr, uint8_t font_base_tile) {

    // BCD type is a uint32_t
    uint8_t * p_score = (uint8_t *) &(state.game_settings.hi_score);

    // Move to right-most tile of score
    p_vram_addr += (SCORE_DIGITS - 1u);

    // TODO: Delete
    // // SCORE_DIGITS should be a multiple of 2
    // for(uint8_t c = 0u; c < SCORE_DIGITS / 2u; c++)  {
    //     // Render two digits into adjacent tile map slots, right -> left
    //     // Score is stored as packed BCD in a uint32_t, one digit per nybble
    //     // Digits are In memory as 0x10, 0x32, 0x54
    //     set_vram_byte(p_vram_addr--, font_base_tile + (*p_score & 0x0Fu));
    //     set_vram_byte(p_vram_addr--, font_base_tile + ((*p_score >> 4u) & 0x0Fu));

    //     // Move to next pair of digits in next byte
    //     p_score++;
    // }


    // Render score from Right to Left in digit pairs
    set_vram_byte(p_vram_addr--, font_base_tile + (*p_score & 0x0Fu));
    set_vram_byte(p_vram_addr--, font_base_tile + ((*(p_score++) >> 4u) & 0x0Fu));

    set_vram_byte(p_vram_addr--, font_base_tile + (*p_score & 0x0Fu));
    set_vram_byte(p_vram_addr--, font_base_tile + ((*(p_score++) >> 4u) & 0x0Fu));

    // Since there are only 5 digits, only extract one from MSByte of BCD score
    set_vram_byte(p_vram_addr--, font_base_tile + (*p_score & 0x0Fu));
}



