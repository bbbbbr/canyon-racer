#ifndef SCORE_H
#define SCORE_H

#include <gbdk/bcd.h>

#define SCORE_DIGITS  6u
#define SCORE_X_START ((160u - (SCORE_DIGITS * 8u)) + 8u) // Sprites are 8 pixels wide, 8 pixel offset
#define SCORE_Y_START ((144u - 16u) + 16u)  // 16 pixel offset for spriets in 8x16 mode


extern BCD score;
extern const BCD score_increment;


#define SCORE_INCREMENT() bcd_add(&score, &score_increment)

void score_reset(void);
void score_update(void);

#endif