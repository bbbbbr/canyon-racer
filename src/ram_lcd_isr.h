
#ifndef _RAM_LCD_ISR_H
#define _RAM_LCD_ISR_H

#define RAM_ISR_SZ_BYTES (44)  // Manually sized for now. Current MapFX Parallax LCD ISR is 44 bytes, Intro credits ISR is 43 bytes

#define FUNC_LENGTH(start_addr, end_addr) ((void *)&(end_addr) - (void *)&(start_addr))

void copy_lcd_isr_to_isr_ram(void * start_addr, void * end_addr);

#endif
