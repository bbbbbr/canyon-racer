
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

// Graphics
#include "../res/map_canyon.h"
#include "../res/sprite_ship.h"

#include "input.h"
#include "common.h"

#include "map_fx.h"

#include "entity_ship.h"



// TODO: move to gfx.x
void init_gfx_map() {

    set_bkg_data(map_canyon_TILE_ORIGIN, map_canyon_TILE_COUNT, map_canyon_tiles);

    if (_cpu == CGB_TYPE) {
        // Set CGB Palette
        // set_bkg_palette(0, nes_num_pals, nes_pal_cgb);
        cgb_compatibility();
        // TODO: CGB Colors
    } else {
        // Set DMG palette
        // BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
    }

    // TODO: Do this right with a full size map?
    set_bkg_tiles(0,0, map_canyon_WIDTH / map_canyon_TILE_W, map_canyon_HEIGHT / map_canyon_TILE_H, map_canyon_map);

}


void init_gfx_sprites() {


    // Load metasprite tile data into VRAM
    set_sprite_data(SPR_TILES_START_SHIP, sprite_ship_TILE_COUNT, sprite_ship_tiles);

    // set_sprite_data((SPR_TILES_START_SHIP), sprite_ship_TILE_COUNT, sprite_ship_tiles);
    // set_sprite_data((SPR_TILES_START_SHIP_CANOPY), sprite_ship_canopy_TILE_COUNT, sprite_ship_canopy_tiles);
    // set_sprite_data((SPR_TILES_START_SHOTS), sprite_shots_TILE_COUNT, sprite_shots_tiles);


    // oam_high_water += move_metasprite(sprite_ship_metasprites[SPR_SHIP_SHADOW],
    //                                  (SPR_TILES_START_SHIP),
    //                                  oam_high_water,
    //                                  ship_x + SPR_SHIP_SHADOW_X_OFS,
    //                                  ship_y + SPR_SHIP_SHADOW_Y_OFS);


    SPRITES_8x16;

    if (_cpu == CGB_TYPE) {
        // Set CGB Palette
        // set_sprite_palette(0, nes_num_pals, nes_pal_cgb);
    } else {
        // Set DMG palette (top two colors BLACK for higher contrast against background)
        // OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_LITE_GRAY, DMG_WHITE);
        OBP0_REG = DMG_PALETTE( DMG_WHITE, DMG_WHITE, DMG_LITE_GRAY, DMG_BLACK);
    }
}


void init_gfx(void) {

    init_gfx_map();
    init_gfx_sprites();

    SHOW_BKG;
    SHOW_SPRITES;

    DISPLAY_ON;
}


void init(void) {

    if (_cpu == CGB_TYPE) {

        // Don't use CGB 2x mode, it throws off timing (unless an additional ISR is implemented)
        // Use 2x CGB speed if available
        // cpu_fast();
    }

    // TODO: bind this to a button press
//    initrand(sys_time);

    // TODO: fade-out
    init_gfx();

    entity_ship_init();

    map_fx_isr_enable();

    // TODO: fade-in
}


uint8_t oam_high_water;

void main() {

    init();

    while (1) {
        wait_vbl_done();

        // == Sprites ==
        oam_high_water = 0;
        oam_high_water = entity_ship_update(oam_high_water);

        // Hide rest of the hardware sprites, because amount of sprites differ between animation frames.
        hide_sprites_range(oam_high_water, SPR_RANGE_END);

        UPDATE_KEYS();
    }
}
