// gameover_screen.h

#ifndef _GAMEOVER_SCREEN_H
#define _GAMEOVER_SCREEN_H

#define GAME_OVER_BIT_SHIFT        0u
#define GAME_OVER_ROTATE_SPEED_NORM (2u << GAME_OVER_BIT_SHIFT) // (2u) looks nice, (1u) is better for non modded screens // In 260/255 degrees
#define GAME_OVER_ROTATE_SPEED_SLOW (1u << GAME_OVER_BIT_SHIFT)

#define GAME_OVER_ADD_LETTER_MASK  (0x0Fu)
#define GAME_OVER_LETTER_RADIUS    (40u)
#define GAME_OVER_SPR_COUNT        (8u)
#define GAME_OVER_SPR_ANGLE_GAP    (255u / (GAME_OVER_SPR_COUNT + 4u)) // 2u)) // (2u is more spaced out, 6u is letters closer together)
#define GAME_OVER_SPR_COUNT_START  (SPR_ID_FREE_START)
#define GAME_OVER_SPR_COUNT_MAX    (SPR_ID_FREE_START +  GAME_OVER_SPR_COUNT)

#define GAME_OVER_X_START          ((160u - (GAME_OVER_LETTER_RADIUS * 2u)) + 8u) // + 8u is just a shade to the right of center due to wave-offset //(160u - (GAME_OVER_LETTER_RADIUS * 2u))

// This would start as far up off-screen as possible
// #define GAME_OVER_Y_START  (255u - (GAME_OVER_LETTER_RADIUS * 2u))

// This starts closer to the center of the screen so there is less delay
#define GAME_OVER_Y_START  (70u - (GAME_OVER_LETTER_RADIUS * 2u))
#define GAME_OVER_Y_END    (144u - (GAME_OVER_LETTER_RADIUS * 2u))

// #define GAME_OVER_START_ANGLE    0
// Start closer to 9pm, combined with GAME_OVER_Y_START mid-screen has a ok swoop in look
#define GAME_OVER_START_ANGLE    (164u)

void gameover_screen_show(uint8_t spr_next_free_tile);


#endif
