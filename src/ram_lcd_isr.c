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
#include <gb/isr.h>
#include <stdint.h>
#include <string.h>

#include "ram_lcd_isr.h"

// Sets up a RAM buffer to store the LCD ISR interrupt handler.
//
// This allows the LCD ISR handler to be switched out without
// the cost of an extra jump / dispatcher.


// CAREFUL: Buffer is manually sized right now
// TODO: auto-sizing was do-able in asm, maybe there is still a way in C
uint8_t ram_lcd_isr_function_buf[RAM_ISR_SZ_BYTES] = {0xD9}; // Default ISR body is single RETI instruction

// Register scanline / STAT ISR handler at the address of the RAM array
ISR_VECTOR(VECTOR_STAT, ram_lcd_isr_function_buf)

// LCD Interrupt should be turned off while using this
void copy_lcd_isr_to_isr_ram(void * start_addr, void * end_addr) {

    // Do not copy if size is too large
    __critical{
        size_t length = end_addr - start_addr;
        if (length <= RAM_ISR_SZ_BYTES)
            memcpy(ram_lcd_isr_function_buf, start_addr, length);
        else
            ram_lcd_isr_function_buf[0] = 0xD9; // Otherwise write a single RETI
    }
}
