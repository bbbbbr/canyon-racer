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

#ifndef SCORE_H
#define SCORE_H

#include <gbdk/bcd.h>

// #define SCORE_DIGITS  6u // Should be multuple of 2, otherwise change from a loop for rendering
#define SCORE_DIGITS  5u

// Score readout location:

// Left Edge? Nope: messes with timing
// #define SCORE_X_START (2u + 8u) // Sprites are 8 pixels wide, 8 pixel offset
// Top edge? Nope: sometimes overlaps with objects
// #define SCORE_Y_START (2u + 16u)  // 16 pixel offset for sprites in 8x16 mode
//
// Lower-Right
#define SCORE_X_START ((160u - (SCORE_DIGITS * 8u)) + 8u) // Sprites are 8 pixels wide, 8 pixel offset
#define SCORE_Y_START ((144u - 16u) + 16u)  // 16 pixel offset for sprites in 8x16 mode

extern const BCD score_increment_amt;


void score_reset(void);
void score_increment(void);
void score_update(void);

void hi_score_render(uint8_t * p_vram_addr, uint8_t font_base_tile);

#endif