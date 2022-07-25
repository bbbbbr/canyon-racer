
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

// #include "entity_ship.h"



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

#define SPR_SHIP_SHADOW_X_OFS 4u
#define SPR_SHIP_SHADOW_Y_OFS 5u
#define SPR_SHIP_SHADOW 2u // index [2] in ship metasprite array
uint8_t ship_sprite_sel = 0u;
uint8_t ship_x = (160  - 10) / 2;
uint8_t ship_y = 255u - 32u;

#define SPR_TILES_START_SHIP 0

uint8_t oam_high_water = 0u;

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
        // Use 2x CGB speed if available
        // cpu_fast();
    }

    // TODO: bind this to a button press
//    initrand(sys_time);

    // TODO: fade-out
    init_gfx();

    map_fx_isr_enable();

    // TODO: fade-in
}


// uint8_t oam_high_water = 0;

// uint8_t last_sys_time = 0;

extern const uint8_t * p_scx_addr;
extern const uint8_t scx_table_1[];
extern uint16_t map_lcd_scx_table_start;
int8_t map_offset_x_last = 0;
uint8_t ship_move_hyst = 0;

void main() {

    init();

    while (1) {
        wait_vbl_done();
        int8_t map_offset_x = 0;


        if (ship_y  < (144 + 32)) {
            // Possible race condtion here, could fix with:
            // int8_t map_offset_x = (int8_t)scx_table_1[map_lcd_scx_table_start + ship_y];
            map_offset_x = (int8_t)p_scx_addr[ship_y];
        } else {
            map_offset_x = (int8_t)p_scx_addr[8];
        }



            if (map_offset_x < map_offset_x_last) {
                ship_sprite_sel = 2u;
                ship_move_hyst = 10u;
            }
            else if (map_offset_x > map_offset_x_last) {
                ship_sprite_sel = 1u;
                ship_move_hyst = 10u;
            }
            else {
                if (ship_move_hyst)
                    ship_move_hyst--;
                else
                    ship_sprite_sel = 0u;
            }

            map_offset_x_last = map_offset_x;

        oam_high_water = 0;
        oam_high_water += move_metasprite(sprite_ship_metasprites[ship_sprite_sel],
                                         (SPR_TILES_START_SHIP),
                                         oam_high_water, ship_x - (uint8_t)map_offset_x, ship_y);
        ship_y -= (sys_time & 0x01u);
        // ship_y = -1; //144u /2;

        // if (sys_time & 0x01) {
        //     move_metasprite(sprite_ship_metasprites[SPR_SHIP_SHADOW],
        //                      (SPR_TILES_START_SHIP),
        //                      oam_high_water,
        //                      (ship_x - p_scx_addr[ship_y]) + SPR_SHIP_SHADOW_X_OFS,
        //                      ship_y + SPR_SHIP_SHADOW_Y_OFS);
        // } else {
        //     hide_sprites_range(oam_high_water, 24u);
        // }

    }
}
