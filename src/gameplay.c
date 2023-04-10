// Copyright 2023 (c) bbbbbr
//
// This software is licensed under:
//
// CC-BY-NC-SA: Attribution Non-Commercial Share Alike Creative Commons
// Attribution-NonCommercial-ShareAlike 4.0 International License
// See: http://creativecommons.org/licenses/by-nc-sa/4.0/
//
// For the purposes of this project "Share Alike" shall also include
// distribution of the source code and any changes to it.

#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "input.h"
#include "common.h"
#include "magic_code.h"
#include "fade.h"
#include "audio.h"
#include "gfx.h"

#include "map_fx.h"

#include "gameover_screen.h"
#include "gameplay_pause.h"
#include "stats.h"
#include "score.h"
#include "level.h"
#include "lives.h"

#include "entity_ship.h"
#include "entity_obstacles.h"

#include "gameplay.h"

#include "../res/sprite_ready.h"

static uint8_t oam_high_water;
static uint8_t oam_high_water_prev;



// Setup before gameplay main loop runs
void gameplay_prestart(void) {

    state.paused = false;
    lives_count_reset();
    lives_display_update();

    // This must be called before mapfx and others
    level_init();

    mapfx_set_gameplay();

    // Random number generator set to a (arbitrary) fixed value
    // at the start of each round so gameplay is deterministic
    initrand(GAMEPLAY_START_RAND_INIT);

    score_reset();
    score_update();

    entity_ship_init();
    entity_obstacles_init();

    // Create initial save point
    // Do it quietly instead of using game_state_save(), without a display update or sound, and before score is shown
    game_state_save();

    delay(50u); // Short delay before fade-in
    fade_in(FADE_DELAY_FX_RUNNING);
}


void gameplay_state_save(void) {
    // Only save state if Lives are not used up
    if (LIVES_COUNT_GET()) {
        game_state_save();
        // Deduct Life / Restore Point (not part of save state)
        LIVES_COUNT_SUBTRACT_ONE();
        audio_sfx_play(SFX_STATE_SAVE_OK); // TODO: SFX:State Save OK
        lives_display_update();
    } else
        audio_sfx_play(SFX_STATE_SAVE_FAIL);  // TODO: SFX:State Save Denied/Fail
}


// Returns: True if restore succeeded
bool gameplay_state_restore(uint8_t spr_next_free_tile, uint8_t oam_high_water) {
    // Only restore state if Lives are not used up
    if (LIVES_COUNT_GET()) {

        // Temporarily pause music so the rest of the gameover track doesn't play after the crash sound // TODO: split crash sound and gameover music?
        audio_music_pause();
        fade_out(FADE_DELAY_STATE_RESTORE_OUT);

        // Restore state and pause map FX
        // Pause must happen AFTER State Restore!
        game_state_restore();
        mapfx_set_setpause(true);

        // Fade in and display animated "READY" notice
        fade_in(FADE_DELAY_STATE_RESTORE_IN);
        gameplay_display_notice(spr_next_free_tile, sprite_ready_TILE_COUNT, sprite_ready_tiles);

        // Deduct Life / Restore Point and update display counters
        LIVES_COUNT_SUBTRACT_ONE();
        lives_display_update();
        score_update();

        // Give the player a moment, then unpause the music and map FX
        delay_lowcpu(GAMEPLAY_DELAY_AFTER_RESTORE);
        // Clear out the notice sprites
        hide_sprites_range(oam_high_water, MAX_HARDWARE_SPRITES);

        audio_sfx_play(SFX_STATE_RESTORE);  // TODO: SFX:State Restore OK
        audio_music_set(MUSIC_GAMEPLAY);
        audio_music_unpause();

        mapfx_set_setpause(false);

        // Set ship to temporary invincibility on restart and center in canyon
        state.ship_counter = SHIP_COUNTER_STARTUP_INVINCIBLE;
        entity_ship_center_in_canyon();
        return true;
    }
    return false;
}


// Main game loop
void gameplay_run(uint8_t spr_next_free_tile) {

    oam_high_water = oam_high_water_prev = SPR_ID_MAX;

    gameplay_display_notice(spr_next_free_tile, sprite_ready_TILE_COUNT, sprite_ready_tiles);
    delay_lowcpu(GAMEPLAY_DELAY_AFTER_RESTORE);
    // Now clear out the notice sprites
    hide_sprites_range(oam_high_water, MAX_HARDWARE_SPRITES);

    while(1) {
        vsync();

        UPDATE_KEYS();

        // == Sprites ==
        oam_high_water = SPR_ID_FREE_START;

        switch ((GET_KEYS_TICKED(BUTTON__STATE_SAVE | BUTTON__PAUSE))) {
            case BUTTON__STATE_SAVE:
                // If still in game play save the state
                // Uses up a life/save state
                if (SHIP_STATE_GET() & SHIP_BITS_PLAYER_ACTIVE) gameplay_state_save();
                break;

            case BUTTON__PAUSE:
                gameplay_pause(spr_next_free_tile, oam_high_water_prev);
                break;
        }

        oam_high_water = entity_obstacles_update(oam_high_water);
        entity_obstacles_check_spawn_new();

        oam_high_water = entity_ship_update(oam_high_water);

        // Clamp to limit
        if (oam_high_water > MAX_HARDWARE_SPRITES) oam_high_water = MAX_HARDWARE_SPRITES;

        // Only need to clear other sprites if more were used last frame
        // Hide rest of the hardware sprites. Amount of sprites differ between animation frames.
        // Now calling this with (prev < current) will just return, no crash
        hide_sprites_range(oam_high_water, oam_high_water_prev);

        oam_high_water_prev = oam_high_water;


        // If game is over, break out and return to main state select
        if (SHIP_STATE_GET() == SHIP_STATE_CRASH_ENDED) {
            // Try to restore if the user has rewinds left
            if (!(gameplay_state_restore(spr_next_free_tile, oam_high_water))) {
                // No more lives. Break out and game over
                break;
            }
        }

        #ifdef VISUAL_DEBUG_BENCHMARK_MAIN
            // VISUAL BENCHMARK END
            // Debug: Benchmark time left by toggling background on/off
            LCDC_REG &= ~LCDCF_BGON; // Turn BG / WIN OFF
        #endif
    }

    // Game Over: Update high score if applicable
    if (!(IS_MAGIC_CODE_ACTIVE))
        stats_update(state.score);
}


