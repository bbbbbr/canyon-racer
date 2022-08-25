#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

// Graphics
// #include "../res/map_canyon.h"
// #include "../res/sprite_ship.h"

#include "input.h"
#include "common.h"
#include "fade.h"

#include "splash_screen.h"

#include "map_fx.h"

#include "score.h"
#include "entity_ship.h"
#include "entity_obstacles.h"

#include "gameplay.h"

// TODO: make global?
uint8_t oam_high_water;
uint8_t oam_high_water_prev;


// Setup before gameplay main loop runs
void gameplay_prestart(void) {

    // Random number generator set to a (arbitrary) fixed value
    // at the start of each round so gameplay is deterministic
    initrand(GAMEPLAY_START_RAND_INIT);

    oam_high_water_prev = SPR_ID_MAX;
    score_reset();
    score_update();

    entity_ship_init();
    entity_obstacles_init();

    delay(50u); // Short delay before fade-in
    fade_in(FADE_DELAY_FX_RUNNING);
}



// Main game loop
void gameplay_run(void) {

    while(1) {
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

        // If game is over, break out and return to main state select
        if (ship_state == SHIP_STATE_GAMEOVER)
            break;

        #ifdef DEBUG_BENCHMARK_BG
            // Debug: Benchmark time left by toggling background source at end of processing
            LCDC_REG |= 0x08u; // Toggle BG source (VBL int for toggle back on)
        #endif
    }
}



// Main game loop
void gameplay_show_gameover(uint8_t bg_next_free_tile) {
    // TODO: show game over screen
    waitpadticked_lowcpu(J_ANY);
}
