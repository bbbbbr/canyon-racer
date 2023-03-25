// input.c

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
