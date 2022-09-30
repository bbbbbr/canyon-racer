#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "input.h"
#include "common.h"
#include "fade.h"

#include "map_fx.h"

#include "stats.h"
#include "score.h"
#include "level.h"

#include "entity_ship.h"
#include "entity_obstacles.h"

#include "gameplay.h"

// TODO: make global?
uint8_t oam_high_water;
uint8_t oam_high_water_prev;


// Setup before gameplay main loop runs
void gameplay_prestart(void) {

    // This must be called before mapfx and others
    level_init();

    mapfx_set_gameplay();

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
        if (SHIP_STATE_GET() == SHIP_STATE_GAMEOVER)
            break;

        #ifdef VISUAL_DEBUG_BENCHMARK_MAIN
            // VISUAL BENCHMARK END
            // Debug: Benchmark time left by toggling background on/off
            LCDC_REG &= ~LCDCF_BGON; // Turn BG / WIN OFF
        #endif
    }

    // Game Over: Update high score if applicable
    stats_update(score);
}



// TODO: move to separate source file: game_over_screen.c


#include "gfx.h"
#include "../res/game_over.h"

// TODO: wait for obstacles to clear off screen? or "Sink" them out to make it faster

// Start gameplay sprites after score sprite (overwrites obstacles/ship/etc)
#define SPR_ID_GAMEOVER_START (SPR_ID_FREE_START)
const uint8_t spr_waves_tbl[] = {0, 1, 2, 3, 3, 2, 1, 0, 0, 1, 2, 3, 3, 2, 1, 0};
// const uint8_t spr_waves_tbl_16[] = {0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1, 0, 0};
// const uint8_t spr_waves_tbl_16[] = {0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 4, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 5, 5, 5, 4, 3, 2, 1, 0, 0};
const uint8_t spr_waves_tbl_16[] = {0, 0, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 6, 5, 4, 3, 2, 1, 0, 0};
const uint8_t * spr_waves_offsets = spr_waves_tbl;

// Show gameover message (while BG continues to run) until user presses a button
void gameplay_show_gameover(uint8_t spr_next_free_tile) {


    static uint8_t y_pos;
    y_pos = 255u - 16u;

  // TODO -- 2X & Verticalize the graphics?
    // GAME
    // OVER

    // TEST: FIXME
    // fade_in(FADE_DELAY_NORM);
    // mapfx_set_intro();

    // TODO: move to gfx.c?
    // Load game-over sprite text
    set_sprite_data(spr_next_free_tile, game_over_TILE_COUNT, game_over_tiles);

    // Show gameover sprites
    // TODO: Animate
    // TODO: do something about Y parallax artifacts to their right?
    uint8_t oam_after_gameover = SPR_ID_FREE_START;
    oam_after_gameover += move_metasprite(game_over_metasprites[0],
                                             (spr_next_free_tile),
                                             oam_after_gameover,
                                             (160u - 80u) / 2,
                                             // (144u - 16u) / 2) + 16u; // TODO: FIXME
                                             y_pos);

    hide_sprites_range(oam_after_gameover, 40u); // TODO: use constant

    // TODO: some kind of letter slide/circle/magnified wave in

    // TODO: fix transition here between Y linear and wave
    // Enqueue next SCX Table to straight so it's a smooth transition


// static uint8_t y_end = 0x44;
// y_end++;

    static uint8_t game_over_LUT_start;
    static uint8_t game_over_LUT_count;
    game_over_LUT_start = 0;
    game_over_LUT_count = 0;
    while (1) {

        wait_vbl_done();
        // spr_waves_offsets = &spr_waves_tbl[(uint8_t)(sys_time >> 2) & 0x07u];
        // spr_waves_offsets = &spr_waves_tbl[(uint8_t)(sys_time >> 3) & 0x07u];

        // if (sys_time & 0x01u) {
        if (game_over_LUT_count++ > 1u) {
            game_over_LUT_count = 0u;
            game_over_LUT_start++;
            // spr_waves_offsets = &spr_waves_tbl[game_over_LUT_start & 0x07u];
            spr_waves_offsets = &spr_waves_tbl_16[game_over_LUT_start & 0x0Fu];

            if ((y_pos > 144u) || (y_pos < 74u))
                y_pos = y_pos + (uint8_t)((int8_t)spr_waves_offsets[0x10u / 2] - 2);  // 0x10u/2 is to center the downward wave motion so it doesn't look left or right edge driven
        }

        // Loop through game-over sprites
        static uint8_t c;
        for (c = (SPR_ID_FREE_START); c < ((SPR_ID_FREE_START) + (game_over_WIDTH / 8u)); c++) {
                // shadow_OAM[c].y = spr_waves_offsets[(c & (uint8_t)0x7u)] + ((144u - 16u) / 2);
            // 7u- to run the wave backward (start at left edge, looks nicer)
            // shadow_OAM[c].y = spr_waves_offsets[7u - (c & (uint8_t)0x7u)] + ((144u - 16u) / 2);
            // shadow_OAM[c].y = spr_waves_offsets[0x0Fu - (c & (uint8_t)0x0Fu)] + ((144u - 16u) / 2) + 16u; // TODO: FIXME
            shadow_OAM[c].y = spr_waves_offsets[0x0Fu - (c & (uint8_t)0x0Fu)] + y_pos; // TODO: FIXME
        }

        UPDATE_KEYS();
        if (KEY_TICKED(J_ANY))
            break;
    }

    // // TODO: show game over screen
    // waitpadticked_lowcpu(J_ANY);

    fade_out(FADE_DELAY_FX_RUNNING);

    // Hide all sprites
    hide_sprites_range(0u, 40u); // TODO: use constant
}

