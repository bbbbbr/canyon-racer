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

#ifndef _STATS_H
#define _STATS_H

#include <gbdk/bcd.h>


void stats_load(void);
void stats_reset(void);
void stats_update(BCD latest_score);


#endif // _STATS_H


