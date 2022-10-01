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