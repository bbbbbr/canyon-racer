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

#ifndef _RAM_LCD_ISR_H
#define _RAM_LCD_ISR_H

#define RAM_ISR_SZ_BYTES (47)  // Manually sized for now. Current MapFX Parallax LCD ISR is 44 bytes, Intro credits ISR is 43 bytes, Help screen ISR is 46

#define FUNC_LENGTH(start_addr, end_addr) ((void *)&(end_addr) - (void *)&(start_addr))

void copy_lcd_isr_to_isr_ram(void * start_addr, void * end_addr);

#endif
