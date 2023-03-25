#include <gbdk/platform.h>
#include <gbdk/bcd.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "common.h"

#include "../res/intro_credits_data.h"
#include "../res/intro_credits_map_comp.h"
#include "../res/intro_credits_tiles_comp.h"

#include "../res/help_screen_data.h"
#include "../res/help_screen_map_comp.h"
#include "../res/help_screen_tiles_comp.h"

#include "../res/splash_logo_map_comp.h"

// Set array size to whichever is largest
// Shared decompression array buffer
#define MAX_DECOM_SZ_INTROCREDITS MAX(intro_credits_map_comp_sz_decomp, intro_credits_tiles_comp_sz_decomp)
#define MAX_DECOM_SZ_HELPSCREEN   MAX(help_screen_map_comp_sz_decomp, help_screen_tiles_comp_sz_decomp)
uint8_t decomp_buf[MAX(MAX_DECOM_SZ_INTROCREDITS, MAX_DECOM_SZ_HELPSCREEN)];



// Options and stats
game_state_data state;
game_state_data state_copy;

uint16_t rand_seed_copy;


void delay_lowcpu(uint16_t num_frames) {

    while (num_frames--) {
        vsync();
    }
}

// NOTE: This only works with a per-scanline STAT ISR running
//       which can wake it up from HALT once per scanline
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


void game_state_save(void) {

    // Long critical sections may delay scanline ISR from running and
    // cause small, noticeable visual glitches. Most vars are only
    // touched by main execution and can be safely copied here.
    //
    // So instead of copying the entire state in a critical section,
    // copy everything, then re-copy just the sensitive vars
    // (gfx which are modified in ISRs) in a critical section

    // Most of main state var doesn't need exclusive locking to copy
    // Struct copy just calls memcpy, but it's easier to read
    state_copy = state;
    rand_seed_copy = __rand_seed;

    // Now just re-copy individual Map FX vars modified by the ISRs
    __critical {
        state_copy.p_scx_table_scanline   = state.p_scx_table_scanline;
        state_copy.p_scx_table_frame_base = state.p_scx_table_frame_base;
        state_copy.p_scx_table_stop       = state.p_scx_table_stop;
        state_copy.p_scx_cur_table        = state.p_scx_cur_table;
    }
}


void game_state_restore(void) {

    // Don't care about visual glitches here from a long
    // critical section since the whole screen often gets
    // changed anyway during the rewind action.
    __critical {
        // Struct copy just calls memcpy, but it's easier to read
        state = state_copy;
        __rand_seed = rand_seed_copy;
    }
}

