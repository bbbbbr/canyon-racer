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

#ifndef _FADE_H
#define _FADE_H

#include "common.h"

#define FADE_DELAY_NORM       (70u)
#define FADE_DELAY_HELP       (120u)
#define FADE_DELAY_INTRO      (40u)  // Little faster than usual
#define FADE_DELAY_FX_RUNNING (15u)  // BG FX use a lot of CPU idle time, so delay between fade steps is smaller
#define FADE_DELAY_STATE_RESTORE_OUT (15u) // Short delay fading out
#define FADE_DELAY_STATE_RESTORE_IN  (5u)  // Very short delay fading in

void fade_in(uint8_t delay_len);
void fade_out(uint8_t delay_len);

#endif
