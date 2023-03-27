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

#ifndef _GAMEPLAY_H
#define _GAMEPLAY_H

#define GAMEPLAY_START_RAND_INIT (0x1234u)
#define GAMEPLAY_DELAY_AFTER_RESTORE 30u    // Wait 1/2 second after restoring state before resuming gameplay

void gameplay_prestart(void);
void gameplay_run(uint8_t spr_next_free_tile);
void gameplay_state_save(void);
bool gameplay_state_restore(uint8_t, uint8_t);

#endif
