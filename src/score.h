#ifndef SCORE_H
#define SCORE_H

#include <gbdk/bcd.h>

// #define SCORE_DIGITS  6u // Should be multuple of 2, otherwise change from a loop for rendering
#define SCORE_DIGITS  5u
#define STATE_RESTORE_DISPLAY_DIGITS  2u

// Score readout location:

// Left Edge? Nope: messes with timing
// #define SCORE_X_START (2u + 8u) // Sprites are 8 pixels wide, 8 pixel offset
// Top edge? Nope: sometimes overlaps with objects
// #define SCORE_Y_START (2u + 16u)  // 16 pixel offset for sprites in 8x16 mode
//
// Lower-Right
#define SCORE_X_START ((160u - (SCORE_DIGITS * 8u)) + 8u) // Sprites are 8 pixels wide, 8 pixel offset
#define SCORE_Y_START ((144u - 16u) + 16u)  // 16 pixel offset for sprites in 8x16 mode

#define STATE_RESTORE_DISPLAY_X_START ((160u - (STATE_RESTORE_DISPLAY_DIGITS * 8u)) + 8u) // Sprites are 8 pixels wide, 8 pixel offset
#define STATE_RESTORE_DISPLAY_Y_START ((144u - 32u) + 16u)  // 16 pixel offset for sprites in 8x16 mode

// Put Rewind/Restore icon a little to the left of the readout
#define STATE_RESTORE_HEADER_X_START ((STATE_RESTORE_DISPLAY_X_START) - (8u + 4u))
#define STATE_RESTORE_HEADER_Y_START (STATE_RESTORE_DISPLAY_Y_START)

extern const BCD score_increment_amt;


#define SCORE_INCREMENT() bcd_add(&state.score, &score_increment_amt)

void score_reset(void);
void score_update(void);

void state_restore_display_update(void);

void hi_score_render(uint8_t * p_vram_addr, uint8_t font_base_tile);

#endif