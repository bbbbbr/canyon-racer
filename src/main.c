
#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "../res/canyon.h"

#include "input.h"
#include "common.h"

#include "map_fx.h"




// TODO: move to gfx.x
void init_gfx_map() {

    set_bkg_data(canyon_TILE_ORIGIN, canyon_TILE_COUNT, canyon_tiles);

    if (_cpu == CGB_TYPE) {
        // Set CGB Palette
        // set_bkg_palette(0, nes_num_pals, nes_pal_cgb);
        cgb_compatibility();
    } else {
        // Set DMG palette
        // BGP_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
    }

    set_bkg_tiles(0,0, canyon_WIDTH / canyon_TILE_W, canyon_HEIGHT / canyon_TILE_H, canyon_map);
// TODO: Do this right with a full size map
    // Set full hardware map for wraparound
    set_bkg_tiles(0,18, canyon_WIDTH / canyon_TILE_W, 32 - (canyon_HEIGHT / canyon_TILE_H), canyon_map);

    // Set up horizon on alternate map (usually used for the Window)
    // set_bkg_data(245u, horizon_tiles_count, horizon_tiles);
    // set_win_submap(0, 0, 32, 4, horizon_map, horizon_map_width);
}


void init_gfx_sprites() {

/*    // Load metasprite tile data into VRAM
    set_sprite_data((SPR_TILES_START_BOULDERS), sprite_boulders_TILE_COUNT, sprite_boulders_tiles);
    set_sprite_data((SPR_TILES_START_SHIP), sprite_ship_TILE_COUNT, sprite_ship_tiles);
    set_sprite_data((SPR_TILES_START_SHIP_CANOPY), sprite_ship_canopy_TILE_COUNT, sprite_ship_canopy_tiles);
    set_sprite_data((SPR_TILES_START_SHOTS), sprite_shots_TILE_COUNT, sprite_shots_tiles);

    SPRITES_8x16;

    if (_cpu == CGB_TYPE) {
        // Set CGB Palette
        set_sprite_palette(0, nes_num_pals, nes_pal_cgb);
    } else {
        // Set DMG palette (top two colors BLACK for higher contrast against background)
        OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_BLACK, DMG_LITE_GRAY, DMG_WHITE);
    }
*/
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
        // Use 2x CGB speed if available
        cpu_fast();
    }

    // TODO: bind this to a button press
//    initrand(sys_time);

    // TODO: fade-out
    init_gfx();

//    map_scroll_init();

    map_fx_isr_enable();

    // TODO: fade-in
}


// uint8_t oam_high_water = 0;

// uint8_t last_sys_time = 0;


void main() {

    init();

    while (1) {
        wait_vbl_done();

//        SCX_REG--;
        // UPDATE_KEYS();
        // if (KEY_TICKED(J_LEFT))
        //     SCX_REG -= 1;
        // if (KEY_TICKED(J_LEFT))
        //     SCY_REG -= 1;
    }
/*    while (1) {
        // TODO: FIXME: HACKAROUND: Don't wait for VBlank if last frame spilled over to the next one
        if (last_sys_time == (uint8_t)sys_time)
            wait_vbl_done();

        last_sys_time = (uint8_t)sys_time;
        // SCX and SCY scroll regs get reset in the vblank ISR and updated in the HBlank ISR

        // == User Input ==
        map_scroll_update();

        // == Sprites ==
        oam_high_water = 0;
        oam_high_water = entity_boulders_update(oam_high_water);
        oam_high_water = entity_ship_update(oam_high_water);
        oam_high_water = entity_shots_update(oam_high_water);

        // Hide rest of the hardware sprites, because amount of sprites differ between animation frames.
        hide_sprites_range(oam_high_water, 40);

        UPDATE_KEYS();

        // while (LY_REG != 139u);
    }
    */
}
