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

#include "../res/splash_logo_map_comp.h"

// Set array size to whichever is largest
// Shared decompression array buffer
uint8_t decomp_buf[MAX(MAX(intro_credits_map_comp_sz_decomp, intro_credits_tiles_comp_sz_decomp), intro_credits_map_comp_sz_decomp)];


// Options and stats
game_state_data state;
game_state_data state_copy;

uint16_t rand_seed_copy;

BCD state_restore_count; // AKA "Rewind"
#define STATE_RESTORE_MAX_BCD  0x99                   // Limit of 99 in BCD
const BCD state_restore_bcd_step_size = MAKE_BCD(01); // 1 in BCD


void delay_lowcpu(uint16_t num_frames) {

    while (num_frames--) {
        wait_vbl_done();
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


// TODO: move into game_state_rewind.c?

void game_state_count_reset(void) {
    state_restore_count = STATE_RESTORE_COUNT_RESET;
}


void game_state_save() {

    // Long critical sections may delay scanline ISR from running and
    // cause small, noticeable visual glitches. Most vars are only
    // touched by main execution and can be safely copied here.
    //
    // So instead of copying the entire state in a critical section,
    // copy everything, then re-copy just the sensitive vars
    // (gfx which are modified in ISRs) in a critical section

    // Most of main state var doesn't need exclusive locking to copy
    memcpy(&state_copy, &state, sizeof(game_state_data));
    rand_seed_copy = __rand_seed;

    // Now just re-copy individual Map FX vars modified by the ISRs
    __critical {
        state_copy.p_scx_table_scanline   = state.p_scx_table_scanline;
        state_copy.p_scx_table_frame_base = state.p_scx_table_frame_base;
        state_copy.p_scx_table_stop       = state.p_scx_table_stop;
        state_copy.p_scx_cur_table        = state.p_scx_cur_table;
    }
}


void game_state_restore() {

        // Should be protected from called when 0, but just to be defensive
    if (state_restore_count) {

        // Don't care about visual glitches here from a long
        // critical section since the whole screen often gets
        // changed anyway during the rewind action.
        __critical {
            memcpy(&state, &state_copy, sizeof(game_state_data));
            __rand_seed = rand_seed_copy;
        }

        // Deduct one from the number used
        // state_restore_count--;
        bcd_sub(&state_restore_count, &state_restore_bcd_step_size);
    }
}


void game_state_count_increment() {

    // Only need to test lsbyte, no need for full BCD test since max is 99
    if ( *(const uint8_t *)&(state_restore_count) != (uint8_t)STATE_RESTORE_MAX_BCD) {
        // state_restore_count++;
        bcd_add(&state_restore_count, &state_restore_bcd_step_size);
    }
}
