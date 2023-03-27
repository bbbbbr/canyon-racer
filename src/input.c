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

#include <gb/gb.h>
#include <gb/cgb.h> // Include cgb functions

#include "input.h"

UINT8 keys = 0x00;
UINT8 previous_keys = 0x00;
UINT8 key_repeat_count = 0x00;

// Reduce CPU usage by only checking once per frame
// Allows a loop control to be passed in
void waitpadticked_lowcpu(UINT8 button_mask) {

    while (1) {

        vsync(); // yield CPU
        UPDATE_KEYS();
        if (KEY_TICKED(button_mask))
            break;
    };

    // Prevent passing through any key ticked
    // event that may have just happened
    UPDATE_KEYS();
}
