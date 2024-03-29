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

#ifndef _GAMEOVER_SCREEN_H
#define _GAMEOVER_SCREEN_H

// Alternate Game over animation that makes it readable on
// non-modded DMG and MGB screens instead of a blur fest
// #define DMG_MGB_LESS_BLUR_STYLE

    #define GAME_OVER_BIT_SHIFT          4u

#ifndef DMG_MGB_LESS_BLUR_STYLE
    // Looks nicer, but not friendly to non-modded DMG/MGB screens
    #define GAME_OVER_ROTATE_SPEED_NORM (2u << GAME_OVER_BIT_SHIFT)
    #define GAME_OVER_Y_DROP_SPEED       1u
    #define GAME_OVER_ADD_LETTER_MASK   (0x0Fu)

#else
    #define GAME_OVER_ROTATE_SPEED_NORM (2u << GAME_OVER_BIT_SHIFT)
    // Non-slow-at-the-top version
        // #define GAME_OVER_Y_DROP_SPEED          2u
    // Medium slowness at top
        // #define GAME_OVER_Y_DROP_SPEED          2u
        // #define GAME_OVER_ROTATE_SPEED_SLOW     (1u << (GAME_OVER_BIT_SHIFT - 1u))
        // #define GAME_OVER_ROTATE_MIDPOINT_BITSHIFT 1u
        // #define GAME_OVER_SLOWDOWN_ANGLE        190u // About 10 o-clock
    // Super slow at top
        #define GAME_OVER_Y_DROP_SPEED             2u
        #define GAME_OVER_ROTATE_MIDPOINT_BITSHIFT 0u
        // #define GAME_OVER_ROTATE_SPEED_SLOW    (1u)
        #define GAME_OVER_ROTATE_SPEED_SLOW       (1u << (GAME_OVER_BIT_SHIFT - 1u))
        #define GAME_OVER_SLOWDOWN_ANGLE           202u // About 9 o-clock
        // #define GAME_OVER_MIDPOINT_HOLD_TIME    15u // looks nice, harder to read
        #define GAME_OVER_MIDPOINT_HOLD_TIME       75u // Easier to read, but maybe too slow
    #define GAME_OVER_ADD_LETTER_MASK              (0x07u)
#endif

#define GAME_OVER_LETTER_RADIUS    (40u)
#define GAME_OVER_SPR_COUNT        (8u)
#define GAME_OVER_SPR_ANGLE_GAP    (255u / (GAME_OVER_SPR_COUNT + 4u)) // 2u)) // (2u is more spaced out, 6u is letters closer together)
#define GAME_OVER_SPR_COUNT_START  (SPR_ID_FREE_START)
#define GAME_OVER_SPR_COUNT_MAX    (SPR_ID_FREE_START +  GAME_OVER_SPR_COUNT)

#if (GAME_OVER_SPR_COUNT_MAX  > 40)
    #error "COMPILE FAIL:START OF SPRITE RANGE TOO LARGE TO FIT GAMEOVER SCREEN SPRITES"
    // Below is optional override
    #undef GAME_OVER_SPR_COUNT_START
    #undef GAME_OVER_SPR_COUNT_MAX
    #define GAME_OVER_SPR_COUNT_START  (40u - GAME_OVER_SPR_COUNT)
    #define GAME_OVER_SPR_COUNT_MAX    (40u)
#endif

#define GAME_OVER_X_START          ((160u - (GAME_OVER_LETTER_RADIUS * 2u)) + 8u) // + 8u is just a shade to the right of center due to wave-offset //(160u - (GAME_OVER_LETTER_RADIUS * 2u))

// This would start as far up off-screen as possible
// #define GAME_OVER_Y_START  (255u - (GAME_OVER_LETTER_RADIUS * 2u))

// This starts closer to the center of the screen so there is less delay
#define GAME_OVER_Y_START  (70u - (GAME_OVER_LETTER_RADIUS * 2u))
#define GAME_OVER_Y_END    (144u - (GAME_OVER_LETTER_RADIUS * 2u))

// #define GAME_OVER_START_ANGLE    0
// Start closer to 9pm, combined with GAME_OVER_Y_START mid-screen has a ok swoop in look
#define GAME_OVER_START_ANGLE    (164u << GAME_OVER_BIT_SHIFT)

void gameover_screen_show(uint8_t spr_next_free_tile);


#endif
