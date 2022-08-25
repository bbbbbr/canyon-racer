// gfx.c

#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

// Graphics
#include "../res/map_canyon.h"
#include "../res/sprite_ship.h"

#include "common.h"
#include "score.h"
#include "map_fx.h"


// TODO: move to gfx.x
uint8_t init_gfx_bg(uint8_t bg_next_free_tile) {

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


uint8_t init_gfx_sprites(uint8_t spr_next_free_tile) {


    // Load metasprite tile data into VRAM
    set_sprite_data(SPR_TILES_START_SHIP, sprite_ship_TILE_COUNT, sprite_ship_tiles);
    set_sprite_data(SPR_TILES_START_OBSTACLES, sprite_obstacles_TILE_COUNT, sprite_obstacles_tiles);
    set_sprite_data(SPR_TILES_START_FONTNUMS, tiles_font_nums_TILE_COUNT, tiles_font_nums_tiles);
    spr_next_free_tile += sprite_ship_TILE_COUNT + sprite_obstacles_TILE_COUNT + tiles_font_nums_TILE_COUNT;


    // TODO: move to after game start
    for (int c = 0u; c < SCORE_DIGITS; c++) {
        set_sprite_tile(SPR_ID_SCORE_START + c, SPR_TILES_FONTNUMS_DIGIT_0);
        move_sprite(SPR_ID_SCORE_START + c, SCORE_X_START + c * 8u, SCORE_Y_START);
    }

    SPRITES_8x16;
    return spr_next_free_tile;
}
