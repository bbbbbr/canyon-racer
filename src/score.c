#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "score.h"

#include <gbdk/bcd.h>

// uint16_t obstacles_cleared_count;
BCD score; //  = MAKE_BCD(00000);
const BCD score_increment = MAKE_BCD(000010);

void score_init(void) {
    score = MAKE_BCD(000000);  // In memory as 0x21 0x43 0x65
    // score = MAKE_BCD(000000);
}


void score_update(void) {

    bcd_add(&score, &score_increment);

    // typedef uint32_t BCD;
    uint8_t * p_score = (uint8_t *) &score;
    uint8_t c = 6;
    while (c != 0)  {
        // Render two digits into adjacent
        // Score is stored as packed BCD in a uint32_t, one digit per nybble
        // 8x16 sprite mode means tile ID's need to be *= 2
        set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score & 0x0Fu) << 1));
        set_sprite_tile(SPR_ID_SCORE_START + (--c), SPR_TILES_START_FONTNUMS + ((*p_score >> (4 - 1)) & (0x0Fu << 1)));  // downshift by 3 instead of 4 to get the (<< 1) at no cost

        // Move to next pair of digits in next byte
        p_score++;
    }
}

