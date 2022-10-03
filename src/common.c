#include <gbdk/platform.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"

// Options and stats
settings_rec game_settings;

void delay_lowcpu(uint16_t num_frames) {

    while (num_frames--) {
        wait_vbl_done();
    }
}

// NOTE: This only works with a per-scanline STAT ISR running
//       which can to wake it up from HALT once per scanline
//
// Waits in halt until a given scanline
void wait_in_halt_to_scanline(uint8_t exit_scanline) {
    do {
        __asm \
            HALT    ; // Wait for any interrupt
            NOP     ; // HALT sometimes skips the next instruction
        __endasm;
    } while (LY_REG != exit_scanline);
}