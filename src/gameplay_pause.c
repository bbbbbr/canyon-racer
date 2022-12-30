#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>

#include "common.h"
#include "input.h"
#include "audio.h"

#include "map_fx.h"

#include "../res/sprite_pause.h"


#define PAUSE_SPR_OFFSCREEN_X  0u
#define PAUSE_SPRITE_COUNT     5u
#define PAUSE_TILES_PER_SPRITE 2u // 8x16 sprites = 2 tiles
#define PAUSE_SPRITE_Y     (((DEVICE_SCREEN_PX_HEIGHT - sprite_pause_HEIGHT) / 2u) -  DEVICE_SPRITE_PX_OFFSET_Y)


// Centering derived from:
// 52u, 52u + (16u * 1u), 52u + (16u * 2u), 52u + (16u * 3u), 52u + (16u * 4u),
// 52u = ((DEVICE_SCREEN_PX_WIDTH - (sprite_pause_WIDTH * (PAUSE_SPRITE_COUNT + PAUSE_SPRITE_COUNT - 1)) / 2u)
const uint8_t pause_sprite_x_frames[] = {

    // Mostly Linear version - (maybe looks a little better)
    // First frame with sprites moving in from edges
/*    0u, 8u, 144u, 160u, 168u,
    20u, 28u, 124u, 140u, 156u,
    44u, 52u, 100u, 116u, 132u,
    // Last frame, Centered
    52u, 68u, 84u, 100u, 116u,
*/
    // Non-Linear version
    // First frame with sprites moving in from edges
    0u,   8u, 144u, 168u, 168u,
    0u,  28u, 124u, 160u, 168u,
    20u, 52u, 100u, 116u, 140u,
    35u, 60u,  92u, 110u, 128u,
    // Last frame, Centered
    52u, 68u,  84u, 100u, 116u,

};


void gameplay_pause(uint8_t spr_next_free_tile, uint8_t oam_high_water) {

    bool game_state_save_queued = false;

    state.paused = true;

    // ====== Pause Effects & Music ======

    // Map effects will stop scrolling during Pause
    mapfx_set_setpause(true);
    audio_music_pause();
    audio_sfx_play(SFX_PAUSE);


    // ====== Animated Pause Notice Sprites ======

    // Load Pause sprite tile data (8x16, 0 - 4 = "PAUSE")
    set_sprite_data(spr_next_free_tile, sprite_pause_TILE_COUNT, sprite_pause_tiles);

    // Assign tiles to sprites, hide all sprites to start with
    // Sprites are assumed to be hidden from previous frame oam cleanup
    for (uint8_t c = 0; c < PAUSE_SPRITE_COUNT; c++) {
        set_sprite_prop(oam_high_water + c, 0u); // Clear sprite attributes
        set_sprite_tile(oam_high_water + c, spr_next_free_tile + (c * PAUSE_TILES_PER_SPRITE));
    }

    // Short animation of PAUSE letters flying in from Left and Right edges
    const uint8_t * p_sprite_x = &pause_sprite_x_frames[0];
    for (uint8_t frame = 0; frame < (ARRAY_LEN(pause_sprite_x_frames) / PAUSE_SPRITE_COUNT); frame++) {

        // Show sprites in next frame location
        for (uint8_t c = 0; c < PAUSE_SPRITE_COUNT; c++) {
            move_sprite(oam_high_water + c, *p_sprite_x++, PAUSE_SPRITE_Y);
        }
        // Wait 3 frames :)
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
    }

    // Hide all other non-score sprites * AFTER * animating the display
    hide_sprites_range(SPR_ID_FREE_START, oam_high_water);


    // ====== Wait for Start Button ======

    // waitpadticked_lowcpu(J_START);
    while (1) {

        wait_vbl_done(); // yield CPU
        UPDATE_KEYS();
        if (KEY_TICKED(BUTTON__PAUSE))
            break;
        else if (KEY_TICKED(BUTTON__STATE_SAVE))
            game_state_save_queued = true;
    };

    // Prevent passing through any key ticked
    // event that may have just happened
    UPDATE_KEYS();

    // Clear out the pause sprites
    hide_sprites_range(oam_high_water, 40u);


    // ====== Resume Effects & Music ======

    audio_music_unpause();
    mapfx_set_setpause(false);

    // if requested handle the state save AFTER mapfx has had it's settings restored
    if (game_state_save_queued)
        game_state_save();

    state.paused = false;
}