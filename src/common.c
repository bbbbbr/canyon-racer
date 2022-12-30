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
const BCD state_restore_count_decrement_amt = MAKE_BCD(00001); // Decrement by 1 each time


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

// TODO: move into game_state_restore.c?

void game_state_count_reset(void) {
    state_restore_count = STATE_RESTORE_COUNT_RESET;
}


void game_state_save() {
    // TODO: Sometimes causes a small noticeable visual glitch
    //       due to the long duration of the critical section memcpy().
    //       Can the main copy be moved out of a critical section and
    //       only be used when copying a couple of sensitive vars, such
    //       as map_fx ones? (ship, obstacles, etc should be ok)
    __critical {
        memcpy(&state_copy, &state, sizeof(game_state_data));
        rand_seed_copy = __rand_seed;
    }
}


void game_state_restore() {
    __critical {
        memcpy(&state, &state_copy, sizeof(game_state_data));
        __rand_seed = rand_seed_copy;
    }
    if (state_restore_count) { // Should be protected from called when 0, but just to be defensive
        // state_restore_count--;
        bcd_sub(&state_restore_count, &state_restore_count_decrement_amt);
    }
}