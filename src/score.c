#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "score.h"


BCD score;
const BCD score_increment = MAKE_BCD(000010);

void score_reset(void) {
    score = MAKE_BCD(000000);
}


void score_update(void) {

    // BCD type is a uint32_t
    uint8_t * p_score = (uint8_t *) &score;
    uint8_t c = SCORE_DIGITS;
    while (c != 0)  {
        // Render two digits into adjacent
        // Score is stored as packed BCD in a uint32_t, one digit per nybble
        // Digits are In memory as 0x10, 0x32, 0x54
        // 8x16 sprite mode means tile ID's need to be *= 2
        set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score & 0x0Fu) << 1));
        set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

        // Move to next pair of digits in next byte
        p_score++;
    }
}

