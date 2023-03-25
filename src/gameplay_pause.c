#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "input.h"
#include "audio.h"
#include "gfx.h"

#include "map_fx.h"
#include "gameplay.h"

#include "../res/sprite_pause.h"


void gameplay_pause(uint8_t spr_next_free_tile, uint8_t oam_high_water) {

    state.paused = true;

    // ====== Pause Effects & Music ======

    // Map effects will stop scrolling during Pause
    mapfx_set_setpause(true);
    audio_music_pause();
    audio_sfx_play(SFX_PAUSE);


    // ====== Animated Pause Notice Sprites ======

    gameplay_display_notice(spr_next_free_tile, sprite_pause_TILE_COUNT, sprite_pause_tiles);

    // ====== Wait for Start Button ======
    waitpadticked_lowcpu(J_START);

    // Prevent passing through any key ticked
    // event that may have just happened
    UPDATE_KEYS();

    // Clear out the pause sprites
    hide_sprites_range(oam_high_water, MAX_HARDWARE_SPRITES);


    // ====== Resume Effects & Music ======

    audio_music_unpause();
    mapfx_set_setpause(false);

    state.paused = false;
}