
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

#include "splash_screen.h"

#include "map_fx.h"

#include "score.h"
#include "entity_ship.h"
#include "entity_obstacles.h"


uint8_t bg_next_free_tile;
uint8_t spr_next_free_tile;


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
    // Or just fill_bkg_tiles()
    set_bkg_tiles(0,0, map_canyon_WIDTH / map_canyon_TILE_W, map_canyon_HEIGHT / map_canyon_TILE_H, map_canyon_map);
    bg_next_free_tile += map_canyon_TILE_COUNT;
}


void init_gfx_sprites() {


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

    bg_next_free_tile = 0u;
    spr_next_free_tile = 0u;

    init_gfx_map();
    init_gfx_sprites();

    SHOW_BKG;
    // SHOW_SPRITES;

    DISPLAY_ON;
}


void init(void) {

    // Random number generator set to a (arbitrary) fixed value
    // so gameplay is deterministic
    initrand(0x1234u);

    if (_cpu == CGB_TYPE) {

        // Don't use CGB 2x mode, it throws off timing (unless an additional ISR is implemented)
        // Use 2x CGB speed if available
        // cpu_fast();
    }

    // TODO: fade-out
    init_gfx();

    entity_ship_init();
    entity_obstacles_init();
    score_reset();

    map_fx_isr_enable();

    // TODO: fade-in
}


// TODO: make global?
uint8_t oam_high_water;
uint8_t oam_high_water_prev;


void main() {

    init();

    splash_intro_run(bg_next_free_tile);

    SHOW_SPRITES;

    oam_high_water_prev = SPR_ID_MAX;

    // Game start sequence, move
    score_update();

    while (1) {
        wait_vbl_done();

        // == Sprites ==
        oam_high_water = SPR_ID_FREE_START;

        oam_high_water = entity_obstacles_update(oam_high_water);
        oam_high_water = entity_ship_update(oam_high_water);

        // hide_sprites_range(0x0Eu, 0x0Fu);
        // Only need to clear other sprites if more were used last frame
        // Plus, calling this with prev < current would cause a crash
        if (oam_high_water_prev > oam_high_water) {
            // Hide rest of the hardware sprites. Amount of sprites differ between animation frames.
            //
            // TODO: FIXME: +1 is a workaround for hide_sprites_range needing: End > Start + 1,
            //       otherwise hide_sprites_range() crashes
            hide_sprites_range(oam_high_water, oam_high_water_prev + 1);
        }
        oam_high_water_prev = oam_high_water;

        UPDATE_KEYS();

        #ifdef DEBUG_BENCHMARK_BG
            // Debug: Benchmark time left by toggling background source at end of processing
            LCDC_REG |= 0x08u; // Toggle BG source (VBL int for toggle back on)
        #endif
    }
}
