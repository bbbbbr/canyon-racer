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

#ifndef __CARTSAVE_H_INCLUDE
#define __CARTSAVE_H_INCLUDE

#define MBC_RAM_BANK_0    0u  // default to RAM Bank 0

void cartsave_restore_data(void);
void cartsave_save_data(void);

#endif
