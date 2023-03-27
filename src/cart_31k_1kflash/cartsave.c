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

#include <gbdk/platform.h>
#include <stdint.h>
#include <string.h>

#include "../common.h"


extern void flash_sector_address;
uint8_t erase_flash() OLDCALL;                                 // erase FLASH sector
uint8_t write_flash(uint8_t * source, uint16_t size) OLDCALL;  // write data to FLASH


void cartsave_restore_data(void) {

    // Copy from flash into game settings
   memcpy((void *)&state.game_settings, &flash_sector_address, sizeof(state.game_settings));
}


void cartsave_save_data(void) {

    // TODO: warning on failure to save?
    // if (!erase_flash()) return 0;
    // return write_flash((void *)&state.game_settings, sizeof(state.game_settings));
   if (erase_flash())
       write_flash((void *)&state.game_settings, sizeof(state.game_settings));
}
