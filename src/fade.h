// fade.h

#ifndef _FADE_H
#define _FADE_H

#include "common.h"

#define FADE_DELAY_NORM       (70u)
#define FADE_DELAY_FX_RUNNING (15u)  // BG FX use a lot of CPU idle time, so delay between fade steps is smalled

void fade_in(uint8_t delay_len);
void fade_out(uint8_t delay_len);

#endif