// gfx.c

#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "gfx.h"
#include "common.h"
#include "score.h"
#include "lives.h"
#include "map_fx.h"

// Graphics
#include "../res/map_canyon.h"
#include "../res/sprite_ship.h"


// TODO: move to gfx.x
uint8_t init_gfx_bg_mapfx(uint8_t bg_next_free_tile) {

    set_bkg_data(map_canyon_TILE_ORIGIN, map_canyon_TILE_COUNT, map_canyon_tiles);

    // if (_cpu == CGB_TYPE) {
    //     // Set CGB Palette
    //     cgb_compatibility();
    // }

    // TODO: Do this right with a full size map?
    // Or just fill_bkg_tiles()
    set_bkg_tiles(0,0, map_canyon_WIDTH / map_canyon_TILE_W, map_canyon_HEIGHT / map_canyon_TILE_H, map_canyon_map);
    bg_next_free_tile += map_canyon_TILE_COUNT;

    return bg_next_free_tile;
}


uint8_t init_gfx_sprites_gameplay(uint8_t spr_next_free_tile) {


    // Load metasprite tile data into VRAM
    set_sprite_data(SPR_TILES_START_SHIP, sprite_ship_TILE_COUNT, sprite_ship_tiles);
    set_sprite_data(SPR_TILES_START_OBSTACLES, sprite_obstacles_TILE_COUNT, sprite_obstacles_tiles);
    set_sprite_data(SPR_TILES_START_FONTNUMS, tiles_font_nums_TILE_COUNT, tiles_font_nums_tiles);
    spr_next_free_tile += sprite_ship_TILE_COUNT + sprite_obstacles_TILE_COUNT + tiles_font_nums_TILE_COUNT;


    // TODO: move to after game start
    // Score digits readout
    for (int c = 0u; c < SCORE_DIGITS; c++) {
        set_sprite_tile(SPR_ID_SCORE_START + c, SPR_TILES_FONTNUMS_DIGIT_0);
        move_sprite(SPR_ID_SCORE_START + c, SCORE_X_START + (c * 8u), SCORE_Y_START);
    }

    // Remaining State Restore Count readout
    for (int c = 0u; c < LIVES_DISPLAY_DIGITS; c++) {
        set_sprite_tile(SPR_ID_LIVES_DISPLAY_START + c, SPR_TILES_FONTNUMS_DIGIT_0);
        move_sprite(SPR_ID_LIVES_DISPLAY_START + c, LIVES_DISPLAY_X_START + (c * 8u), LIVES_DISPLAY_Y_START);
    }
    // "R" Rewind/Restore count indicator
    set_sprite_tile(SPR_ID_LIVES_HEADER_START, SPR_TILES_FONTNUMS_DIGIT_R);
    move_sprite(SPR_ID_LIVES_HEADER_START, LIVES_HEADER_X_START, LIVES_HEADER_Y_START);

    return spr_next_free_tile;
}


// Centering derived from:
// 52u, 52u + (16u * 1u), 52u + (16u * 2u), 52u + (16u * 3u), 52u + (16u * 4u),
// #define PAUSE_SPRITE_COUNT     (sprite_pause_TILE_COUNT / 2) // Divide by 2 for 8x16 sprite mode = number of sprites
// 52u = ((DEVICE_SCREEN_PX_WIDTH - (sprite_pause_WIDTH * (PAUSE_SPRITE_COUNT + PAUSE_SPRITE_COUNT - 1)) / 2u)
const uint8_t gameplay_notice_sprite_x_frames[] = {

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



void gameplay_display_notice(uint8_t spr_next_free_tile, uint8_t oam_high_water, uint8_t tile_count, uint8_t * tiles_addr) {


    // Load notice sprite tile data (8x16)
    set_sprite_data(spr_next_free_tile, tile_count, tiles_addr);

    // Reduce tile count to number of sprites
    uint8_t sprite_count = tile_count / GAMEPLAY_NOTICE_TILES_PER_SPRITE;

    // Use sprites at end of hardware range
    uint8_t message_start_sprite = (MAX_HARDWARE_SPRITES - sprite_count);

    // Assign tiles to sprites, hide all sprites to start with
    // Sprites are assumed to be hidden from previous frame oam cleanup
    for (uint8_t c = 0; c < sprite_count; c++) {
        set_sprite_prop(message_start_sprite + c, 0u); // Clear sprite attributes
        set_sprite_tile(message_start_sprite + c, spr_next_free_tile + (c * GAMEPLAY_NOTICE_TILES_PER_SPRITE));
    }

    // Short animation of PAUSE letters flying in from Left and Right edges
    const uint8_t * p_sprite_x = &gameplay_notice_sprite_x_frames[0];
    for (uint8_t frame = 0; frame < (ARRAY_LEN(gameplay_notice_sprite_x_frames) / sprite_count); frame++) {

        // Show sprites in next frame location
        for (uint8_t c = 0; c < sprite_count; c++) {
            move_sprite(message_start_sprite + c, *p_sprite_x++, GAMEPLAY_NOTICE_SPRITE_Y);
        }
        // Wait 3 frames :)
        wait_vbl_done();
        wait_vbl_done();
        wait_vbl_done();
    }

    // Hide all other non-score sprites * AFTER * animating the display
    hide_sprites_range(SPR_ID_FREE_START, message_start_sprite);
}