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
#include <stdbool.h>

#include "common.h"
#include "input.h"

#include "magic_code.h"

const uint8_t magic_code_seq[] = {J_UP, J_UP, J_DOWN, J_DOWN, J_LEFT, J_RIGHT, J_LEFT, J_RIGHT, J_B, J_A};
uint8_t magic_code_state = MAGIC_CODE_STATE_RESET;


void magic_code_reset(void) {
    magic_code_state = MAGIC_CODE_STATE_RESET;
}


bool magic_code_update(void) {

    // Only check button input if the mode has not already been activated
    if (magic_code_state != MAGIC_CODE_STATE_ACTIVATED) {

        // Check to see if the next button press in the sequence
        // happened during the current frame
        if ( KEY_TICKED( magic_code_seq[ magic_code_state ] ) ) {
            magic_code_state++;

            // If the end of the sequence has been reached
            // then set the code as activated and play a sound
            if (magic_code_state == ARRAY_LEN(magic_code_seq)) {

                magic_code_state = MAGIC_CODE_STATE_ACTIVATED;
                return true;
            }
        }
    }
    return false;
}