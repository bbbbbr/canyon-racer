#include <gbdk/platform.h>
#include <gbdk/metasprites.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>

#include "input.h"
#include "common.h"
#include "fade.h"
#include "audio.h"

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

// TODO: make global?
uint8_t oam_high_water;
uint8_t oam_high_water_prev;



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

    oam_high_water_prev = SPR_ID_MAX;
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
    // Only save state if they haven't used them all up
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
bool gameplay_state_restore(void) {
    // Only restore state if lives / restore points aren't all used up
    if (LIVES_COUNT_GET()) {

        game_state_restore();

        // Deduct Life / Restore Point
        LIVES_COUNT_SUBTRACT_ONE();

        audio_sfx_play(SFX_STATE_RESTORE_OK);  // TODO: SFX:State Restore OK
        audio_music_set(MUSIC_GAMEPLAY);
        audio_music_unpause();
        score_update();
        lives_display_update();

        // Set ship to temporary invincibility on restart and center in canyon
        state.ship_counter = SHIP_COUNTER_STARTUP_INVINCIBLE;
        entity_ship_center_in_canyon();
        return true;
    }
    return false;
}


// Main game loop
void gameplay_run(uint8_t spr_next_free_tile) {

    while(1) {
        wait_vbl_done();

        UPDATE_KEYS();

        // TODO: consider moving into functions
        switch ((GET_KEYS_TICKED(BUTTON__STATE_SAVE | BUTTON__PAUSE))) {
            case BUTTON__STATE_SAVE:
                // If still in game play save the state
                // Uses up a life/save state
                if (SHIP_STATE_GET() & SHIP_BITS_PLAYER_ACTIVE) gameplay_state_save();
                break;

            case BUTTON__PAUSE:
                gameplay_pause(spr_next_free_tile, oam_high_water);
                break;
        }

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


        // If game is over, break out and return to main state select
        if (SHIP_STATE_GET() == SHIP_STATE_CRASH_ENDED) {
            // Try to restore if the user has rewinds left
            if (gameplay_state_restore()) {
                // TODO: consider displaying a "GET READY" type indicator on restore (and dropping pause support?)
            }
            else
                // Otherwise gameover, break out and game over
                break;
        }

        #ifdef VISUAL_DEBUG_BENCHMARK_MAIN
            // VISUAL BENCHMARK END
            // Debug: Benchmark time left by toggling background on/off
            LCDC_REG &= ~LCDCF_BGON; // Turn BG / WIN OFF
        #endif
    }

    // Game Over: Update high score if applicable
    stats_update(state.score);
}


